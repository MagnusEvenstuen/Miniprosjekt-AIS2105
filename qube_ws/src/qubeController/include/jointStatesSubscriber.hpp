#ifndef JOINTSTATESUBSCRIBER_HPP
#define JOINTSTATESUBSCRIBER_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "velocityController.hpp"

class jointStateSubscriber : public rclcpp::Node {
public:
    jointStateSubscriber() : Node("jointStateSubscriber"), velocityController(this){
        declare_parameter("wantedAngle", 5.0);
        wantedAngle_ = get_parameter("wantedAngle").as_double();
        velocityController.setWantedAngle(wantedAngle_);
        
        //Stores the new value when changes occure
        add_on_set_parameters_callback(
            [this](const std::vector<rclcpp::Parameter>& params) {
                auto result = rcl_interfaces::msg::SetParametersResult();
                result.successful = true;
                for (const auto& param : params) {
                    if (param.get_name() == "wantedAngle") {
                        wantedAngle_ = param.as_double();
                        RCLCPP_INFO(get_logger(), "Parameter updated: target_angle = %f", wantedAngle_);
                        //Sets the wanted angle in the velocity controller
                        velocityController.setWantedAngle(wantedAngle_);
                    }
                }
                return result;
            });

        //Subscriber
        jointSubscriber = create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            //Calls joint callback when a new message arrives
            std::bind(&jointStateSubscriber::jointCallback, this, std::placeholders::_1));
    }
    //This isn't used
    double getWantedAngle(){
        return wantedAngle_;
    }

private:
    void jointCallback(const sensor_msgs::msg::JointState::SharedPtr msg) {
        //Checks if position and velocity is excisting values
        if (!msg->position.empty() && !msg->velocity.empty()) {
            position_ = msg->position[0];
            //The speed value isn't used
            speed_ = msg->velocity[0];
            velocityController.pidController(position_);
        }
    }

    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr jointSubscriber;
    VelocityController velocityController;
    float position_;
    float speed_;
    double wantedAngle_;
};

#endif // JOINTSTATESUBSCRIBER_HPP