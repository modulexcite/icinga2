/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "ido/hostdbobject.h"
#include "ido/dbtype.h"
#include "ido/dbvalue.h"
#include "icinga/host.h"
#include "icinga/service.h"
#include "icinga/notification.h"
#include "icinga/checkcommand.h"
#include "icinga/eventcommand.h"
#include "icinga/compatutility.h"
#include "base/objectlock.h"
#include <boost/foreach.hpp>

using namespace icinga;

REGISTER_DBTYPE("Host", "host", 1, HostDbObject);

HostDbObject::HostDbObject(const String& name1, const String& name2)
	: DbObject(DbType::GetByName("Host"), name1, name2)
{ }

Dictionary::Ptr HostDbObject::GetConfigFields(void) const
{
	Dictionary::Ptr fields = boost::make_shared<Dictionary>();
	Host::Ptr host = static_pointer_cast<Host>(GetObject());

	Service::Ptr service = host->GetHostCheckService();

	if (!service)
		return Empty;

	Dictionary::Ptr attrs;

	{
		ObjectLock olock(service);
		attrs = CompatUtility::GetServiceConfigAttributes(service, CompatTypeHost);
	}

	fields->Set("alias", attrs->Get("alias"));
	fields->Set("display_name", attrs->Get("display_name"));

	fields->Set("address", attrs->Get("address"));
	fields->Set("address6", attrs->Get("address6"));

	fields->Set("check_command_object_id", service->GetCheckCommand());
	fields->Set("check_command_args", Empty);
	fields->Set("eventhandler_command_object_id", service->GetEventCommand());
	fields->Set("eventhandler_command_args", Empty);
	fields->Set("notification_timeperiod_object_id", Notification::GetByName(attrs->Get("notification_period")));
	fields->Set("check_timeperiod_object_id", service->GetCheckPeriod());
	fields->Set("failure_prediction_options", Empty);

	fields->Set("check_interval", attrs->Get("check_interval"));
	fields->Set("retry_interval", attrs->Get("retry_interval"));
	fields->Set("max_check_attempts", attrs->Get("max_check_attempts"));

	fields->Set("first_notification_delay", Empty);
	fields->Set("notification_interval", attrs->Get("notification_interval"));
	fields->Set("notify_on_down", attrs->Get("notify_on_down"));
	fields->Set("notify_on_unreachable", attrs->Get("notify_on_unreachable"));
	fields->Set("notify_on_recovery", attrs->Get("notify_on_recovery"));
	fields->Set("notify_on_flapping", attrs->Get("notify_on_flapping"));
	fields->Set("notify_on_downtime", attrs->Get("notify_on_downtime"));

	fields->Set("stalk_on_up", Empty);
	fields->Set("stalk_on_down", Empty);
	fields->Set("stalk_on_unreachable", Empty);

	fields->Set("flap_detection_enabled", attrs->Get("flap_detection_enabled"));
	fields->Set("flap_detection_on_up", Empty);
	fields->Set("flap_detection_on_down", Empty);
	fields->Set("flap_detection_on_unreachable", Empty);
	fields->Set("low_flap_threshold", attrs->Get("low_flap_threshold"));
	fields->Set("high_flap_threshold", attrs->Get("high_flap_threshold"));
	fields->Set("process_performance_data", attrs->Get("process_performance_data"));
	fields->Set("freshness_checks_enabled", attrs->Get("check_freshness"));
	fields->Set("freshness_threshold", Empty);
	fields->Set("passive_checks_enabled", attrs->Get("passive_checks_enabled"));
	fields->Set("event_handler_enabled", attrs->Get("event_handler_enabled"));
	fields->Set("active_checks_enabled", attrs->Get("active_checks_enabled"));
	fields->Set("retain_status_information", 1);
	fields->Set("retain_nonstatus_information", 1);
	fields->Set("notifications_enabled", attrs->Get("notifications_enabled"));
	fields->Set("obsess_over_host", 0);
	fields->Set("failure_prediction_enabled", 0);

	fields->Set("notes", attrs->Get("notes"));
	fields->Set("notes_url", attrs->Get("notes_url"));
	fields->Set("action_url", attrs->Get("action_url"));
	fields->Set("icon_image", attrs->Get("icon_image"));
	fields->Set("icon_image_alt", attrs->Get("icon_image_alt"));
	fields->Set("statusmap_image", attrs->Get("statusmap_image"));
	fields->Set("have_2d_coords", attrs->Get("have_2d_coords"));
	fields->Set("x_2d", attrs->Get("x_2d"));
	fields->Set("y_2d", attrs->Get("y_2d"));
	/* deprecated in 1.x */
	fields->Set("have_3d_coords", 0);

	return fields;
}

Dictionary::Ptr HostDbObject::GetStatusFields(void) const
{
	Dictionary::Ptr fields = boost::make_shared<Dictionary>();
	Host::Ptr host = static_pointer_cast<Host>(GetObject());
	Service::Ptr service = host->GetHostCheckService();

	if (!service)
		return Empty;

	Dictionary::Ptr attrs;

	{
		ObjectLock olock(service);
		attrs = CompatUtility::GetServiceStatusAttributes(service, CompatTypeHost);
	}

	fields->Set("output", attrs->Get("plugin_output"));
	fields->Set("long_output", attrs->Get("long_plugin_output"));
	fields->Set("perfdata", attrs->Get("performance_data"));
	fields->Set("current_state", attrs->Get("current_state"));
	fields->Set("has_been_checked", attrs->Get("has_been_checked"));
	fields->Set("should_be_scheduled", attrs->Get("should_be_scheduled"));
	fields->Set("current_check_attempt", attrs->Get("current_attempt"));
	fields->Set("max_check_attempts", attrs->Get("max_attempts"));
	fields->Set("last_check", DbValue::FromTimestamp(attrs->Get("last_check")));
	fields->Set("next_check", DbValue::FromTimestamp(attrs->Get("next_check")));
	fields->Set("check_type", Empty);
	fields->Set("last_state_change", DbValue::FromTimestamp(attrs->Get("last_state_change")));
	fields->Set("last_hard_state_change", DbValue::FromTimestamp(attrs->Get("last_hard_state_change")));
	fields->Set("last_time_up", DbValue::FromTimestamp(attrs->Get("last_time_up")));
	fields->Set("last_time_down", DbValue::FromTimestamp(attrs->Get("last_time_down")));
	fields->Set("last_time_unreachable", DbValue::FromTimestamp(attrs->Get("last_time_unreachable")));
	fields->Set("state_type", attrs->Get("state_type"));
	fields->Set("last_notification", DbValue::FromTimestamp(attrs->Get("last_notification")));
	fields->Set("next_notification", DbValue::FromTimestamp(attrs->Get("next_notification")));
	fields->Set("no_more_notifications", Empty);
	fields->Set("notifications_enabled", attrs->Get("notifications_enabled"));
	fields->Set("problem_has_been_acknowledged", attrs->Get("problem_has_been_acknowledged"));
	fields->Set("acknowledgement_type", attrs->Get("acknowledgement_type"));
	fields->Set("current_notification_number", attrs->Get("current_notification_number"));
	fields->Set("passive_checks_enabled", attrs->Get("passive_checks_enabled"));
	fields->Set("active_checks_enabled", attrs->Get("active_checks_enabled"));
	fields->Set("eventhandler_enabled", attrs->Get("eventhandler_enabled"));
	fields->Set("flap_detection_enabled", attrs->Get("flap_detection_enabled"));
	fields->Set("is_flapping", attrs->Get("is_flapping"));
	fields->Set("percent_state_change", attrs->Get("percent_state_change"));
	fields->Set("latency", attrs->Get("check_latency"));
	fields->Set("execution_time", attrs->Get("check_execution_time"));
	fields->Set("scheduled_downtime_depth", attrs->Get("scheduled_downtime_depth"));
	fields->Set("failure_prediction_enabled", Empty);
	fields->Set("process_performance_data", attrs->Get("process_performance_data"));
	fields->Set("obsess_over_host", Empty);
	fields->Set("modified_host_attributes", Empty);
	fields->Set("event_handler", attrs->Get("event_handler"));
	fields->Set("check_command", attrs->Get("check_command"));
	fields->Set("normal_check_interval", attrs->Get("check_interval"));
	fields->Set("retry_check_interval", attrs->Get("retry_interval"));
	fields->Set("check_timeperiod_object_id", service->GetCheckPeriod());

	return fields;
}
