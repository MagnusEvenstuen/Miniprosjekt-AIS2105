#ifndef VELOCITYCONTROLLER_HPP
#define VELOCITYCONTROLLER_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

class VelocityController{
public:
    VelocityController(rclcpp::Node* node){       
        //Creates the publisher
        speedAngelPublisher_ = node->create_publisher<std_msgs::msg::Float64MultiArray>(
            "/velocity_controller/commands", 10);
        //The timer for the publisher, publishes every 10 milliseconds
        publishTimer_ = node->create_wall_timer(
            std::chrono::milliseconds(10),
            [this]() {
                auto msg = std_msgs::msg::Float64MultiArray();
                msg.data.push_back(valueToPublish_);
                speedAngelPublisher_->publish(msg);
            });
    }

    void setWantedAngle(double wantedAngle){
        wantedAngle_ = wantedAngle;
    }

    //Could use dt as an input, but I just assume it will be almost constant
    void pidController(float actualAngle){
        float pError = wantedAngle_ - actualAngle;
        float dError = pError - lastError;
        integral_ += pError;
        //Sets the value to publish
        valueToPublish_ = pError * p_ + dError * d_ + integral_ * i_;
        lastError = pError;
    }

private:
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr speedAngelPublisher_;
    rclcpp::TimerBase::SharedPtr publishTimer_;
    float wantedAngle_;
    float integral_ = 0;
    float p_ = 1.2;
    float i_ = 0.001;
    float d_ = 0.1;
    float lastError = 0;
    float valueToPublish_ = 0;
};

#endif // VELOCITYCONTROLLER_HPP