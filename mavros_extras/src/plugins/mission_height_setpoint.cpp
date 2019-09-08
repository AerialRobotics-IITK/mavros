#include <mavros/mavros_plugin.h>
#include <mavros_msgs/MissionHeightSetpoint.h>

namespace mavros
{
namespace extra_plugins
{
class MissionHeightSetpointPlugin : public plugin::PluginBase
{
public:
    MissionHeightSetpointPlugin() : PluginBase(),
                                     status_nh("~mission_height_setpoint")
    {
    }

    void initialize(UAS &uas_)
    {
        PluginBase::initialize(uas_);

        status_sub = status_nh.subscribe("mission_height_setpoint", 10, &MissionHeightSetpointPlugin::status_cb, this);
    }

    Subscriptions get_subscriptions()
    {
        return {/* Rx disabled */};
    }

private:
    ros::NodeHandle status_nh;
    ros::Subscriber status_sub;

    /**
	 * @param req	received MissionHeightSetpoint msg
	 */
    void status_cb(const mavros_msgs::MissionHeightSetpoint::ConstPtr &req)
    {
        mavlink::common::msg::MISSION_HEIGHT_SETPOINT mission{};

        mission.height = req->height;
        
        UAS_FCU(m_uas)->send_message_ignore_drop(mission);
    }
};
} // namespace extra_plugins
} // namespace mavros

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::MissionHeightSetpointPlugin, mavros::plugin::PluginBase)