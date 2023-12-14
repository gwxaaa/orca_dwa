#include "ros/ros.h"
#include "gazebo_msgs/ModelStates.h"
#include "gazebo_msgs/ModelState.h"
#include "std_msgs/String.h"
#include "ModelSubPub.h"
#include "Agent.h"
#include "Neighbor.h"
#include <cmath>
#include <KinematicModel.h>
#include "DWA.h"
#include "Vector2.h"
#include "Line.h"
namespace RVO
{
  ModelSubPub::ModelSubPub(const std::string &modelName, double time, gazebo_msgs::ModelState target_model_state,
                           geometry_msgs::Pose goal_pose, double maxSpeed_, double neighborDistance_, double timeHorizon_, double radius_, double num,
                           double max_linear_speed, double max_angular_speed)
      : modelName_(modelName),
        time(time),
        maxSpeed_(maxSpeed_),
        neighborDistance_(neighborDistance_),
        timeHorizon_(timeHorizon_),
        radius_(radius_),
        goal_pose(goal_pose),
        target_model_state(target_model_state),
        lastStoredNewVelocity(agentVelocity),
        num(num),
        max_linear_speed(max_linear_speed),
        max_angular_speed(max_angular_speed),
        newVelocities(1, Vector2(0, 0))

  {
    // 初始化ROS节点
    ros::NodeHandle nh;
    target_model_ = modelName;
    model_states_sub_ = nh.subscribe("/gazebo/model_states", 10, &ModelSubPub::modelStatesCallback, this);
    model_states_pub_ = nh.advertise<gazebo_msgs::ModelState>("/gazebo/set_model_state", 10);
  }
  // 回调函数，处理模型状态信息
  void ModelSubPub::modelStatesCallback(const gazebo_msgs::ModelStates::ConstPtr &msg)
  {
    other_models_states.clear();
    // gazebo_msgs::ModelState target_model_state;
    // 遍历所有模型
    for (size_t i = 0; i < msg->name.size(); ++i)
    {
      if (msg->name[i] == target_model_)
      {
        // 存储特定目标模型的信
        target_model_state.model_name = msg->name[i];
        target_model_state.pose = msg->pose[i];
        target_model_state.twist = msg->twist[i];
      }
      else if (msg->name[i] != "ground_plane")
      {
        // 存储其他模型的信息
        gazebo_msgs::ModelState other_model_state;
        other_model_state.model_name = msg->name[i];
        other_model_state.pose = msg->pose[i];
        other_model_state.twist = msg->twist[i];
        other_models_states.push_back(other_model_state);
      }
    }
    // 目标模型信息格式的转换，终点信息的转换
    std::string agentname = target_model_;
    agentpose = target_model_state.pose;
    agenttwist = target_model_state.twist;
    // 格式转化
    Vector2 agentPosition(agentpose.position.x, agentpose.position.y);
    double deltaTheta = agenttwist.angular.z * time;
    // 这个是这一次的转换角度
    double velocityX = agenttwist.linear.x * cos(deltaTheta);
    double velocityY = agenttwist.linear.x * sin(deltaTheta);
    Vector2 agentVelocity(velocityX, velocityY);
    Vector2 goalPosition(goal_pose.position.x, goal_pose.position.y);
    double velocityX1 = (goalPosition.x() - agentPosition.x()) * 0.1;
    double velocityY1 = (goalPosition.y() - agentPosition.y()) * 0.1;
    Vector2 prefVelocity(velocityX1, velocityY1);
    // 障碍物信息的存储
    obstacle_poses.clear(); // 清空 obstacle_poses，确保它是空的
    // 将 other_models_states 中的数据转移到 obstacle_poses 中
    for (const auto &model_state : other_models_states)
    {
      geometry_msgs::Pose pose = model_state.pose;
      obstacle_poses.push_back(pose);
    }
    // 开始计算ORCA算法
    RVO::Neighbor neighborobject(*this);
    // // 获取计算后的邻居信息
    std::vector<RVO::Agent *> agentNeighbors_ = neighborobject.getAgentNeighbors();
    std::vector<RVO::Obstacle *> obstacleNeighbors_ = neighborobject.getObstacleNeighbors();
    RVO::Agent agent(agentPosition, agentVelocity, prefVelocity, time, maxSpeed_, neighborDistance_, timeHorizon_, other_models_states, radius_);

    const std::vector<RVO::Line> &resultlines = agent.computeNewVelocity(agentPosition, agentVelocity, prefVelocity, agentNeighbors_,
                                                                         obstacleNeighbors_, time);

    // 输出ORCA线段集合
    // 此时得到的是速度障碍集，他所谓的线段和点，都是将速度障碍集的边界描述出来。
    // 既然已经知道边界，那么就可以求解下一刻，那个速度可以成为可行的。

    // 位置信息转换为速度信息，求得速度可行集
    // 是输出最大速度还是可行速度集合----因为多边形，同时运动矢量，所以如何保证结果是在可行集范围内。
    Vector2 maxVelocityBoundary = getVelocityToReachPolygon(resultlines, currentPosition, time);
    reachableVelocitiesResult = getVelocitiesWithinBounds(orcaLines, currentPosition, time, minVelocity, maxVelocity);
    // 将速度可行集进行格式转换，输入给DWA算法
    // 如果输出的是速度可行集，那么转换----循环？
    std::vector<double> linearSpeeds;
    std::vector<double> angularSpeeds;
    // 遍历 reachableVelocitiesResult 中的每个速度向量
    for (const Vector2 &velocity : reachableVelocitiesResult)
    {
      // 对每个速度向量进行速度值和角速度值的计算
      double x = velocity.x();
      double y = velocity.y();
      double linearSpeed = sqrt(x * x + y * y);
      double theta = atan2(y, x);
      double angularSpeed = (theta - 0) / time;
      // 将计算得到的速度值和角速度值添加到对应的向量中
      linearSpeeds.push_back(linearSpeed);
      angularSpeeds.push_back(angularSpeed);
    }
    RVO::DWAPlanner planner(goal_pose, obstacle_poses, max_linear_speed, max_angular_speed, time, num, agentpose);
    // 查找最佳速度，final_pose，最佳分数
    const geometry_msgs::Twist &best_twist = planner.FindBestTwist(agentpose);
    geometry_msgs::Pose final_pose;
    KinematicModel kinematic_model(agentpose, best_twist);
    final_pose = kinematic_model.calculateNewPosition(time);
    // 发布信息
    gazebo_msgs::ModelState model_state;
    model_state.model_name = agentname;
    model_state.pose = final_pose;
    model_states_pub_.publish(model_state);
  }
  std::vector<gazebo_msgs::ModelState> ModelSubPub::getothermodels() const
  {
    return other_models_states;
  };

  std::vector<Vector2> ModelSubPub::getReachableVelocitiesResult() const
  {
    return reachableVelocitiesResult;
  };
  // 计算给定ORCA多边形的速度边界
  // 计算到达多边形的最大速度边界
  bool is_inside_polygon(const Vector2 &point, const std::vector<Vector2> &polygonVertices)
  {
    bool inside = false;
    size_t i, j = polygonVertices.size() - 1;

    for (i = 0; i < polygonVertices.size(); i++)
    {
      if ((polygonVertices[i].y() > point.y()) != (polygonVertices[j].y() > point.y()) &&
          (point.x() < (polygonVertices[j].x() - polygonVertices[i].x()) * (point.y() - polygonVertices[i].y()) /
                               (polygonVertices[j].y() - polygonVertices[i].y()) +
                           polygonVertices[i].x()))
      {
        inside = !inside;
      }
      j = i;
    }

    return inside;
  }

  Vector2 find_leftmost_vertex(const std::vector<Vector2> &polygonVertices)
  {
    if (polygonVertices.empty())
    {
      // Return a default Vector2 in case the input vector is empty
      return Vector2(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    }

    Vector2 leftmostVertex = polygonVertices[0]; // Initialize with first vertex
    for (const Vector2 &vertex : polygonVertices)
    {
      if (vertex.x() < leftmostVertex.x())
      {
        leftmostVertex = vertex;
      }
    }
    return leftmostVertex;
  }
  // 计算两条线的交点
  Vector2 calculate_intersection(const Line &line1, const Line &line2)
  {
    double x1 = line1.point.x();
    double y1 = line1.point.y();
    double x2 = line1.point.x()+ line1.direction.x();
    double y2 = line1.point.y() + line1.direction.y();

    double x3 = line2.point.x();
    double y3 = line2.point.y();
    double x4 = line2.point.x() + line2.direction.x();
    double y4 = line2.point.y() + line2.direction.y();

    double determinant = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (determinant == 0)
    {
      // Lines are parallel or coincident, handle accordingly
      // Return some default value or handle exceptional case
      return Vector2(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    }
    else
    {
      double intersectionX = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / determinant;
      double intersectionY = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / determinant;

      return Vector2(intersectionX, intersectionY);
    }
  }

  // 按顺时针连接多边形顶点
  std::vector<Vector2> connect_points_clockwise(const Vector2 &startVertex, const std::vector<Vector2> &polygonVertices)
  {
    std::vector<Vector2> convexPolygonVertices;

    // 找到起始点的索引
    size_t startIndex = 0;
    for (size_t i = 0; i < polygonVertices.size(); ++i)
    {
      if (polygonVertices[i] == startVertex)
      {
        startIndex = i;
        break;
      }
    }

    // 从 startVertex 出发，按照顺时针顺序依次连接多边形的顶点
    size_t currentIndex = startIndex;
    do
    {
      convexPolygonVertices.push_back(polygonVertices[currentIndex]);
      size_t nextIndex = (currentIndex + 1) % polygonVertices.size();

      // 寻找与当前点连接的下一个顶点，这里可以根据实际需求确定连接的逻辑
      double minAngle = std::numeric_limits<double>::max();
      size_t minAngleIndex = nextIndex;

      for (size_t i = nextIndex; i < nextIndex + polygonVertices.size(); ++i)
      {
        size_t index = i % polygonVertices.size();

        if (index == currentIndex || index == (currentIndex + 1) % polygonVertices.size())
        {
          continue;
        }

        Vector2 vector1 = polygonVertices[(currentIndex + 1) % polygonVertices.size()] - polygonVertices[currentIndex];
        Vector2 vector2 = polygonVertices[index] - polygonVertices[currentIndex];

        double angle = std::atan2(vector2.y(), vector2.x()) - std::atan2(vector1.y(), vector1.x());
        if (angle < 0.0)
        {
          angle += 2.0 * M_PI;
        }

        if (angle < minAngle)
        {
          minAngle = angle;
          minAngleIndex = index;
        }
      }

      currentIndex = minAngleIndex;
    } while (currentIndex != startIndex);

    return convexPolygonVertices;
  }
  std::vector<Vector2> convert_orca_lines_to_polygon(const std::vector<Line> &orcaLines)
  {
    std::vector<Vector2> polygonVertices;

    // 计算所有交点
    for (size_t i = 0; i < orcaLines.size(); ++i)
    {
      for (size_t j = i + 1; j < orcaLines.size(); ++j)
      {
        Vector2 intersectionPoint = calculate_intersection(orcaLines[i], orcaLines[j]);
        if (intersectionPoint.x() != std::numeric_limits<double>::infinity() &&
            intersectionPoint.y() != std::numeric_limits<double>::infinity() &&
            is_inside_polygon(intersectionPoint, polygonVertices))
        {
          polygonVertices.push_back(intersectionPoint);
        }
      }
    }
    // 找到最左侧的交点作为起始点
    Vector2 startVertex = find_leftmost_vertex(polygonVertices);
    // 按顺时针或逆时针连接点，形成多边形
    std::vector<Vector2> convexPolygon = connect_points_clockwise(startVertex, polygonVertices);
    return convexPolygon;
  }

  // Vector2 getVelocityToReachPolygon(const std::vector<RVO::Line> &orca_lines, const Vector2 &currentPosition, double time)
  // {
  //   Vector2 maxVelocity(0.0, 0.0);
  //   // 对每条ORCA线段进行遍历
  //   for (const RVO::Line &line : orca_lines)
  //   {
  //     Vector2 direction = line.direction();
  //     // 计算当前速度边界的时间因素
  //     double timeFactor = 1.0;
  //     Vector2 velocity = direction * timeFactor;
  //     // 更新最大速度边界
  //     if (velocity.abs() > maxVelocity.abs())
  //     { // 假设 abs() 函数返回向量的长度
  //       maxVelocity = velocity;
  //     }
  //   }
  //   return maxVelocity;
  // }
  // std::vector<Vector2> getVelocitiesWithinBounds(const std::vector<RVO::Line> &orca_lines, const Vector2 &currentPosition,
  //                                                double time, const Vector2 &minVelocity, const Vector2 &maxVelocity)
  // {
  //   std::vector<Vector2> reachableVelocities;
  //   // 获取模型能够达到 ORCA 多边形边缘的速度边界限制
  //   Vector2 maxVelocityBoundary = getVelocityToReachPolygon(orca_lines, currentPosition, time);
  //   // 获取最小速度边界和最大速度边界
  //   Vector2 minVelocityBoundary = minVelocity;
  //   Vector2 actualMaxVelocityBoundary = maxVelocityBoundary;
  //   // 如果给定的最大速度边界小于 ORCA 计算的最大速度边界，更新为最大速度边界
  //   if (maxVelocity.x() < maxVelocityBoundary.x() || maxVelocity.y() < maxVelocityBoundary.y())
  //   {
  //     actualMaxVelocityBoundary = maxVelocity;
  //   }
  //   // 以最大速度边界为限制，生成介于最小速度矢量和最大速度边界之间的一系列速度向量
  //   double increment = 0.1; // 调整速度矢量的步长
  //   for (double vx = minVelocityBoundary.x(); vx <= actualMaxVelocityBoundary.x(); vx += increment)
  //   {
  //     for (double vy = minVelocityBoundary.y(); vy <= actualMaxVelocityBoundary.y(); vy += increment)
  //     {
  //       Vector2 velocity(vx, vy);

  //       // 将速度向量加入结果集合
  //       reachableVelocities.push_back(velocity);
  //     }
  //   }
  //   return reachableVelocities;
  // }

  // 目前这个是只能检测出当前速度·是否符合，并根据约束进行调整。
  // 这一个是根据约束实现得到可行速度，但是只能得到一个，或者几个。无法说对所有可能都进行验证。
  std::vector<Vector2> compute_feasible_velocities(const Agent &agent, const std::vector<Line> &orcaLines)
  {
    std::vector<Vector2> feasible_velocities;

    for (const Line &line : orcaLines)
    {
      // 计算当前智能体与ORCA线的相对位置和速度
      Vector2 relativePosition = agent.position_ - line.point;
      Vector2 relativeVelocity = agent.velocity_ - line.direction;
      // 计算垂直于ORCA线的方向
      Vector2 perpendicularDirection(-line.direction.y(), line.direction.x());
      // 计算当前速度与ORCA线的投影
      double projection = relativeVelocity * line.direction;
      double perpendicularProjection = relativeVelocity * perpendicularDirection;
      // 判断当前速度是否在ORCA线的左侧（也即是否遵循ORCA线的约束）
      if (projection >= 0.0 && projection * projection <= relativeVelocity * relativeVelocity)
      {
        // 判断当前速度是否在ORCA线的范围内
        double dotProduct = relativeVelocity * relativePosition;
        double discriminant = dotProduct * dotProduct - relativePosition * relativePosition +
                              line.direction * line.direction;
        if (discriminant >= 0.0)
        {
          // 计算可行速度在ORCA线方向上的分量
          Vector2 feasibleVelocity = projection * line.direction - relativeVelocity;
          // 将满足ORCA线约束的速度添加到可行速度集合中
          feasible_velocities.push_back(feasibleVelocity);
        }
      }
      else if (projection < 0.0 && perpendicularProjection >= 0.0)
      {
        // 当速度在ORCA线右侧时，根据垂直方向进行调整
        float abs = std::sqrt(agent.velocity_.x() * agent.velocity_.x() + agent.velocity_.y() * agent.velocity_.y());
        Vector2 perpendicularDirection = {-line.direction.y(), line.direction.x()};
        Vector2 feasibleVelocity = {
            perpendicularDirection.x() * line.direction.x() * abs,
            perpendicularDirection.y() * line.direction.y() * abs,
        };
        feasible_velocities.push_back(feasibleVelocity);
      }
    }

    return feasible_velocities;
  }
}
