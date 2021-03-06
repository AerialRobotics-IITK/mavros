/**
 * @brief Companion Status plugin
 * @file companion_status.cpp
 * @author Tanja Baumann <tanja@auterion.com>
 *
 * @addtogroup plugin
 * @{
 */
/*
 * Copyright 2018 Tanja Baumann.
 *
 * This file is part of the mavros package and subject to the license terms
 * in the top-level LICENSE file of the mavros repository.
 * https://github.com/mavlink/mavros/tree/master/LICENSE.md
 */

#include <mavros/mavros_plugin.h>

#include <mavros_msgs/GripperServo.h>

namespace mavros
{
namespace extra_plugins
{

//! Mavlink enumerations
// using mavlink::common::;
// using mavlink::common::MAV_STATE;
// using mavlink::common::MAV_TYPE;
// using utils::enum_value;

/**
 * @brief Obstacle companion process status plugin
 *
 * Publishes the status of components running on the companion computer
 * @see status_cb()
 */
class GripperServoPlugin : public plugin::PluginBase
{
public:
    GripperServoPlugin() : PluginBase(),
                                     status_nh("~gripper_servo")
    {
    }

    void initialize(UAS &uas_)
    {
        PluginBase::initialize(uas_);

        status_sub = status_nh.subscribe("gripper_servo", 10, &GripperServoPlugin::status_cb, this);
    }

    Subscriptions get_subscriptions()
    {
        return {/* Rx disabled */};
    }

private:
    ros::NodeHandle status_nh;
    ros::Subscriber status_sub;

    /**
	 * @brief Send companion process status to FCU over a heartbeat message
	 *
	 * Message specification: https://mavlink.io/en/messages/common.html#HEARTBEAT
	 * @param req	received GripperServo msg
	 */
    void status_cb(const mavros_msgs::GripperServo::ConstPtr &req)
    {
        mavlink::common::msg::GRIPPER_SERVO gripper{};

        gripper.servo_setpoint = req->servo_setpoint;
       
        // ROS_DEBUG_STREAM_NAMED("companion_process_status", "companion process component id: " << utils::to_string_enum<MAV_COMPONENT>(req->component) << " companion process status: " << utils::to_string_enum<MAV_STATE>(heartbeat.system_status) << std::endl
        //                                                                                           << heartbeat.to_yaml());

        UAS_FCU(m_uas)->send_message_ignore_drop(gripper);
    }
};
} // namespace extra_plugins
} // namespace mavros

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::GripperServoPlugin, mavros::plugin::PluginBase)
