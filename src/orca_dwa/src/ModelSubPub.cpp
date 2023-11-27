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
    // 位置信息转换为速度信息，求得速度可行集
    reachableVelocitiesResult = getVelocitiesWithinBounds(orcaLines, currentPosition, time, minVelocity, maxVelocity);
    // 将速度可行集进行格式转换，输入给DWA算法
    double X = maxVelocity.x();
    double Y = maxVelocity.y();
    max_linear_speed = sqrt(X * X + Y * Y);
    double theta = atan2(Y, X);
    max_angular_speed = (theta - 0) / time;

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

  std::vector<Vector2> ModelSubPub::getVelocitiesWithinBounds(const std::vector<RVO::Line> &orca_lines, const Vector2 &currentPosition,
                                                              double time, const Vector2 &minVelocity, const Vector2 &maxVelocity)
  {
    std::vector<Vector2> reachableVelocities;
    // 获取模型能够达到 ORCA 多边形边缘的速度边界限制
    Vector2 maxVelocityBoundary = getVelocityToReachPolygon(orca_lines, currentPosition, time);

    // 以最大速度边界为限制，生成介于最小速度矢量和最大速度边界之间的一系列速度向量
    double increment = 0.1; // 调整速度矢量的步长
    for (double vx = minVelocity.x(); vx <= maxVelocityBoundary.x(); vx += increment)
    {
      for (double vy = minVelocity.y(); vy <= maxVelocityBoundary.y(); vy += increment)
      {
        Vector2 velocity(vx, vy);

        // 确保速度向量在最大速度边界限制内
        if (velocity.x() <= maxVelocityBoundary.x() && velocity.y() <= maxVelocityBoundary.y())
        {
          // 将速度向量加入结果集合
          reachableVelocities.push_back(velocity);
        }
      }
    }

    return reachableVelocities;
  };
  std::vector<Vector2> ModelSubPub::getReachableVelocitiesResult() const
  {
    return reachableVelocitiesResult;
  };
  std::vector<Vector2> ModelSubPub::getVelocitiesWithinBounds(const std::vector<RVO::Line> &orca_lines, const Vector2 &currentPosition,
                                                            double time, const Vector2 &minVelocity, const Vector2 &maxVelocity)
{
    std::vector<Vector2> reachableVelocities;
    // 获取模型能够达到 ORCA 多边形边缘的速度边界限制
    Vector2 maxVelocityBoundary = getVelocityToReachPolygon(orca_lines, currentPosition, time);

    // 获取最小速度边界和最大速度边界
    Vector2 minVelocityBoundary = minVelocity;
    Vector2 actualMaxVelocityBoundary = maxVelocityBoundary;

    // 如果给定的最大速度边界小于 ORCA 计算的最大速度边界，更新为最大速度边界
    if (maxVelocity.x() < maxVelocityBoundary.x() || maxVelocity.y() < maxVelocityBoundary.y()) {
        actualMaxVelocityBoundary = maxVelocity;
    }

    // 以最大速度边界为限制，生成介于最小速度矢量和最大速度边界之间的一系列速度向量
    double increment = 0.1; // 调整速度矢量的步长
    for (double vx = minVelocityBoundary.x(); vx <= actualMaxVelocityBoundary.x(); vx += increment)
    {
        for (double vy = minVelocityBoundary.y(); vy <= actualMaxVelocityBoundary.y(); vy += increment)
        {
            Vector2 velocity(vx, vy);

            // 将速度向量加入结果集合
            reachableVelocities.push_back(velocity);
        }
    }

    return reachableVelocities;
}
}
