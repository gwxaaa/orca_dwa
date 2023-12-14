#ifndef MODEL_SUB_PUB_H
#define MODEL_SUB_PUB_H
#include "ros/ros.h"
#include "gazebo_msgs/ModelStates.h"
#include "gazebo_msgs/SetModelState.h"
#include "std_msgs/String.h"
#include "Agent.h"
#include "Obstacle.h"
#include "Neighbor.h"
#include <KinematicModel.h>
#include "Line.h"
#include "Vector2.h"

namespace RVO
{

  class ModelSubPub
  {
  public:
    ModelSubPub(const std::string &modelName, double time, gazebo_msgs::ModelState target_model_state, geometry_msgs::Pose goal_pose,
                double maxSpeed_, double neighborDistance_, double timeHorizon_, double radius_, double num,
                double max_linear_speed, double max_angular_speed);
    // 回调函数，处理模型状态信息
    void modelStatesCallback(const gazebo_msgs::ModelStates::ConstPtr &msg);
    std::vector<gazebo_msgs::ModelState> getothermodels() const;
    std::vector<Vector2> getVelocitiesWithinBounds(const std::vector<RVO::Line> &orca_lines, const Vector2 &currentPosition,
                                                   double time, const Vector2 &minVelocity, const Vector2 &maxVelocity);
    Vector2 getVelocityToReachPolygon(const std::vector<RVO::Line> &orca_lines, const Vector2 &currentPosition, double time);
    std::vector<Vector2> getReachableVelocitiesResult() const;
    double radius_; // 避障半径
  private:
    ros::NodeHandle nh;
    ros::Subscriber model_states_sub_;
    ros::Publisher model_states_pub_;
    std::string target_model_; // 用于存储目标模型的名称
    double time;
    double num;
    geometry_msgs::Pose target_model_pose;
    double new_velocity;
    geometry_msgs::Pose agentpose;
    geometry_msgs::Twist agenttwist;
    geometry_msgs::Pose goal_pose;
    std::vector<gazebo_msgs::ModelState> other_models_states;
    std::string modelName_;
    double time_;
    double targetModelSpeed_;
    double goalModelSpeed_;
    double maxSpeed_;
    double neighborDistance_;
    double timeHorizon_;
    std::vector<Agent *> agentNeighbors_;
    std::vector<Agent *> obstacleNeighbors_;
    gazebo_msgs::ModelState target_model_state;
    Vector2 agentPosition;
    Vector2 agentVelocity;
    Vector2 goalPosition;
    geometry_msgs::Pose new_pose;
    geometry_msgs::Twist new_twist;
    geometry_msgs::Pose final_pose;
    Vector2 lastvelocity;
    Vector2 prevelocity;
    Vector2 lastStoredNewVelocity;
    std::vector<Vector2> newVelocities;
    double max_linear_speed;
    double max_angular_speed; // 用于存储新速度的数组
    std::vector<geometry_msgs::Pose> obstacle_poses;

    std::vector<RVO::Line> orcaLines;
    Vector2 currentPosition;
    Vector2 minVelocity;
    Vector2 maxVelocity;
    std::vector<Vector2> reachableVelocitiesResult;
  };
} // namespace RVO
#endif // MODEL_SUB_PUB_H