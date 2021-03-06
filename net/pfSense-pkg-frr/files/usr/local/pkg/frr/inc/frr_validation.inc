<?php
/*
 * frr_validation.inc
 *
 * part of pfSense (https://www.pfsense.org)
 * Copyright (c) 2017 Rubicon Communications, LLC (Netgate)
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* General use validation functions */

/* The definiteion of a WORD in FRR is not very clear, make it a general
 * function so that we can alter the validation later.
 */
function frr_validate_word($word) {
	/* For the time being, use is_hostname() because any hostname is a valid
	 * WORD, though it is technically too strict as a WORD can include some
	 * things that a hostname cannot. If a better set of validation rules
	 * is discovered, change this validation later. */
	return is_hostname($word);
}

/* Validate a community as AS:VAL pairs */
function frr_validate_community_asvalpair($community) {
	list ($as, $val) = explode(":", $community);
	if (is_numeric($as) && is_numeric($val) &&
	    (($as >= 0) && ($as <= 65535)) &&
	    (($val >= 0) && ($val <= 65535))) {
		return true;
	}
	return false;
}

function frr_validate_ulong($value, $startat = 0) {
	/* 0-4294967295 */
	return (strlen($value) > 0) && frr_validate_intrange($value, $startat, 4294967295);
}

function frr_validate_asnum($value) {
	return frr_validate_ulong($value, 1);
}

function frr_validate_intrange($value, $min, $max) {
	if (is_numericint($value) && ($value >= $min) && ($value <= $max)) {
		return $value;
	} else {
		return null;
	}
}

function frr_validate_intrange_input($descr, $value, $min, $max) {
	global $input_errors;
	if (($value <> "") && frr_validate_intrange($value, $min, $max) == null) {
		$input_errors[] = sprintf(gettext("%s must be an integer between %s and %s."), $descr, $min, $max);
	}
}

function frr_validate_community_asval_list($arr) {
	global $frr_well_known_communities;
	foreach (explode(' ', $arr) as $avp) {
		if (!frr_validate_community_asvalpair($avp) && !in_array($avp, $frr_well_known_communities)) {
			/* If there is an invalid community, then fail. */
			return false;
		}
	}
	return true;
}

function frr_validate_bgp_origin($name) {
	$valid_origins = array('egp', 'igp', 'incomplete');
	if (in_array($name, $valid_origins)) {
		return true;
	} else {
		return false;
	}
}

function frr_validate_bgp_sourceproto($name) {
	$valid_sources = array('bgp', 'ospf', 'ospf6', 'connected', 'kernel', 'static', 'system', 'rip', 'ripng', 'isis');
	if (in_array($name, $valid_sources)) {
		return true;
	} else {
		return false;
	}
}

/* GUI input validation functions, one per page */

function frr_global_validate_input() {
	global $config, $input_errors;

	if (($_POST['password'] <> "") && (strpos($_POST['password'], "'") !== false)) {
		$input_errors[] = "Password cannot contain a single quote (')";
	}
	if (!empty($_POST['routerid']) && !is_ipaddrv4($_POST['routerid'])) {
		$input_errors[] = "Router ID must be an address.";
	}
}

/* Read in rowhelper data from $_POST, sort it by sequence number, and put it back in POST. */
function frr_sort_rows_by_seq() {
	global $config, $pkg;
	$pkgarr = array();

	/* Inside this foreach was lifted from pkg_edit.php. It reads rowhelper values
	 * from $_POST which are separate and end in a number (e.g. name0, name1),
	 * into a typical array. */
	foreach ($pkg['fields']['field'] as $fields) {
		if ($fields['type'] == "rowhelper") {
			$rowhelpername="row";
			foreach ($fields['rowhelper']['rowhelperfield'] as $rowhelperfield) {
				foreach ($_POST as $key => $value) {
					$matches = array();
					if (preg_match("/^{$rowhelperfield['fieldname']}(\d+)$/", $key, $matches)) {
						if ($rowhelperfield['type'] == "textarea") {
							$pkgarr[$rowhelpername][$matches[1]][$rowhelperfield['fieldname']] = unixnewlines($value);
						} else {
							$pkgarr[$rowhelpername][$matches[1]][$rowhelperfield['fieldname']] = $value;
						}
					}
				}
			}
		}
	}

	/* Sort the rowhelper data by "seq" column */
	frr_array_seq_sort($pkgarr['row']);

	/* Take the sorted data and push it back into $_POST before it gets saved. */
	foreach ($pkgarr['row'] as $idx => $row) {
		foreach (array_keys($row) as $keyname) {
			$_POST["{$keyname}{$idx}"] = $row[$keyname];
		}
	}
}

function frr_global_acls_validate_input() {
	global $config, $input_errors, $pkg;
	frr_sort_rows_by_seq();
}

function frr_global_prefixes_validate_input() {
	global $config, $input_errors, $pkg;
	frr_sort_rows_by_seq();

	if (empty($_POST['name'])) {
		$input_errors[] = "A name is required.";
	}

	foreach ($_POST as $key => $value) {
		$matches = array();
		if (preg_match("/^seq(\d+)$/", $key, $matches)) {
			if (!is_numeric($_POST["seq{$matches[1]}"])) {
				$input_errors[] = "Sequence in row {$matches[1]} must be a number.";
			}
		}
		if (preg_match("/^action(\d+)$/", $key, $matches)) {
			if (!in_array($_POST["action{$matches[1]}"], array('deny', 'permit'))) {
				$input_errors[] = "Action in row {$matches[1]} must be Deny or Permit.";
			}
		}
		if (preg_match("/^source(\d+)$/", $key, $matches)) {
			if (!is_subnet($_POST["source{$matches[1]}"]) && ($_POST["source{$matches[1]}"] != 'any')) {
				$input_errors[] = "Network in row {$matches[1]} must be a subnet.";
			}
		}
		if (preg_match("/^ge(\d+)$/", $key, $matches)) {
			if (!empty($_POST["ge{$matches[1]}"]) && !is_numeric($_POST["ge{$matches[1]}"])) {
				$input_errors[] = "Minimum prefix in row {$matches[1]} must be a number.";
			} elseif (isset($_POST["le{$matches[1]}"]) &&
				    is_numeric($_POST["le{$matches[1]}"]) &&
				    ( (int) $_POST["ge{$matches[1]}"] + 1 >= (int) $_POST["le{$matches[1]}"] )) {
				$input_errors[] = "Minimum prefix in row {$matches[1]} must be less than maximum, and differ by more than 1.";
			}
		}
		if (preg_match("/^le(\d+)$/", $key, $matches)) {
			if (!empty($_POST["le{$matches[1]}"]) && !is_numeric($_POST["le{$matches[1]}"])) {
				$input_errors[] = "Maximum prefix in row {$matches[1]} must be a number.";
			}
		}
	}

}

function frr_global_routemaps_validate_input() {
	global $config, $input_errors;

	/* Do not allow a name of: ACCEPTFILTER */
}

function frr_global_raw_validate_input() {
	global $config, $input_errors;

	/* Run commands with -C, grep -c '\*\*\* error' */
}

function frr_ospf_areas_validate_input() {
	global $config, $id, $input_errors;
	/* area must be an IP address */
	if (!is_ipaddr($_POST['area'])) {
		$input_errors[] = "Area must be a valid ip_address.";
	}

	/* type must be ('none', 'stub', 'nosum', 'nssa', 'nssanosum') */
	if (!empty($_POST['type']) && !in_array($_POST['type'], array('none', 'stub', 'nosum', 'nssa', 'nssanosum'))) {
		$input_errors[] = "Invalid Area Type.";
	}

	/* defaultcost 0-16777215 */
	frr_validate_intrange_input("Default Route Cost", $_POST['defaultcost'], 0, 16777215);

	/* authentication must be ("", "digest", "simple") */
	if (!empty($_POST['authtype']) && !in_array($_POST['authtype'], array('digest', 'simple'))) {
		$input_errors[] = "Invalid Authentication type.";
	}

	/* range array */
	foreach ($_POST as $key => $value) {
		$matches = array();

		/* rangeprefix must be a subnet */
		if (preg_match("/^rangeprefix(\d+)$/", $key, $matches)) {
			if (!empty($_POST["rangeprefix{$matches[1]}"]) &&
			    !is_subnetv4($_POST["rangeprefix{$matches[1]}"])) {
				$input_errors[] = "Summary Prefix in row {$matches[1]} must be a subnet.";
			}
		}

		/* rangecost 0-16777215 */
		if (preg_match("/^rangecost(\d+)$/", $key, $matches)) {
			frr_validate_intrange_input("Summary Cost in row {$matches[1]}", $_POST['rangecost{$matches[1]}'], 0, 16777215);
			if (($_POST['rangecost{$matches[1]}'] <> "") &&
			    (!empty($_POST['notadvertise{$matches[1]}']) ||
			    !empty($_POST['subprefix{$matches[1]}']))) {
				$input_errors[] = "Only one of Do Not Advertise, Cost, or Substitution Prefix may be chosen in row {$matches[1]}.";
			}
		}

		/* subprefix must be a subnet */
		if (preg_match("/^subprefix(\d+)$/", $key, $matches)) {
			if (($_POST['subprefix{$matches[1]}'] <> "") &&
			    !is_subnetv4($_POST["subprefix{$matches[1]}"])) {
				$input_errors[] = "Substitute Prefix in row {$matches[1]} must be a subnet.";
			}
			if (!empty($_POST['subprefix{$matches[1]}']) &&
			    (!empty($_POST['notadvertise{$matches[1]}']) ||
			    !empty($_POST['rangecost{$matches[1]}']))) {
				$input_errors[] = "Only one of Do Not Advertise, Cost, or Substitution Prefix may be chosen in row {$matches[1]}.";
			}
		}
	}
}

function frr_ospf_interfaces_validate_input() {
	global $config, $id, $input_errors;

	if ($config['installedpackages']['frrospfdinterfaces']['config']) {
		foreach ($config['installedpackages']['frrospfdinterfaces']['config'] as $index => $conf) {
			if ($index == 0) {
				continue;
			}
			if ($id != $index && $conf['interface'] == $_POST['interface']) {
				$input_errors[] = "Interface {$_POST['interface']} is already configured.";
			}
		}
	}
	frr_validate_intrange_input("Cost", $_POST['metric'], 1, 65535);
	if (!empty($_POST['interfacearea']) && !is_ipaddr($_POST['interfacearea'])) {
		$input_errors[] = "Area must be a valid ip_address.";
	}
	if (!empty($_POST['networktype']) && !in_array($_POST['networktype'], array('broadcast', 'non-broadcast', 'point-to-multipoint', 'point-to-point'))) {
		$input_errors[] = "Invalid interface type.";
	}
	if (!empty($_POST['authtype']) && !in_array($_POST['authtype'], array('digest', 'simple', 'null'))) {
		$input_errors[] = "Invalid authentication type.";
	}
	switch ($_POST['authtype']) {
		case "digest":
			if (empty($_POST['password'])) {
				$input_errors[] = "Please input a password.";
			} elseif (strlen($_POST['password']) > 16) {
				$input_errors[] = "Message Digest Password must be 16 characters or less.";
			}
			break;
		case "simple":
			if (empty($_POST['password'])) {
				$input_errors[] = "Please input a password.";
			} elseif (strlen($_POST['password']) > 8) {
				$input_errors[] = "Simple Password must be 8 characters or less.";
			}
			break;
		default:
			break;
	}
	frr_validate_intrange_input("Router Priority", $_POST['routerpriorityelections'], 0, 255);
	frr_validate_intrange_input("Retransmit Interval", $_POST['retransmitinterval'], 3, 65535);
	frr_validate_intrange_input("Hello Interval", $_POST['hellointervalinseconds'], 1, 65535);
	frr_validate_intrange_input("Dead Interval", $_POST['deadtimer'], 1, 65535);
	frr_validate_intrange_input("Minimal Hello", $_POST['minimalhello'], 1, 10);
	if (!empty($_POST['minimalhello']) && (!empty($_POST['deadtimer']) || !empty($_POST['hellointervalinseconds']))) {
		$input_errors[] = "Minimal Hello suppresses Hello Interval and Dead Interval.";
	}
}

function frr_ospf_validate_input() {
	global $config, $input_errors;

	if (!$config['installedpackages']['frrospfdinterfaces']['config']) {
		$input_errors[] = "Please select an interface to use for FRR OSPFd.";
	}

	if (!empty($_POST['routerid']) && !is_ipaddr($_POST['routerid'])) {
		$input_errors[] = "Router ID must be an address.";
	}
	if (!is_ipaddr($_POST['area'])) {
		$input_errors[] = "Area must be a valid ip_address.";
	}

	/* spfholdtime 0-60000 */
	frr_validate_intrange_input("SPF Hold Time", $_POST['spfholdtime'], 0, 60000);
	/* spfdelay 0-600000 */
	frr_validate_intrange_input("SPF Delay", $_POST['spfdelay'], 0, 60000);

	/* redistributeconnectedsubnets_metric (0-16777214) */
	frr_validate_intrange_input("Connected Metric", $_POST['redistributeconnectedsubnets_metric'], 0, 16777214);
	/* redistributeconnectedsubnets_metrictype (1-2) */
	frr_validate_intrange_input("Connected Metric Type", $_POST['redistributeconnectedsubnets_metrictype'], 1, 2);

	/* redistributekernel_metric (0-16777214) */
	frr_validate_intrange_input("Kernel Metric", $_POST['redistributekernel_metric'], 0, 16777214);
	/* redistributekernel_metrictype (1-2) */
	frr_validate_intrange_input("Kernel Metric Type", $_POST['redistributekernel_metrictype'], 1, 2);

	/* redistributebgp_metric (0-16777214) */
	frr_validate_intrange_input("BGP Metric", $_POST['redistributebgp_metric'], 0, 16777214);
	/* redistributebgp_metrictype (1-2) */
	frr_validate_intrange_input("BGP Metric Type", $_POST['redistributebgp_metrictype'], 1, 2);

	/* redistributestatic_metric (0-16777214) */
	frr_validate_intrange_input("Static Metric", $_POST['redistributestatic_metric'], 0, 16777214);
	/* redistributestatic_metrictype (1-2) */
	frr_validate_intrange_input("Static Metric Type", $_POST['redistributestatic_metrictype'], 1, 2);

	/* redistributedefaultroute_metric (0-16777214) */
	frr_validate_intrange_input("Default Route Metric", $_POST['redistributedefaultroute_metric'], 0, 16777214);
	/* redistributedefaultroute_metrictype (1-2) */
	frr_validate_intrange_input("Default Route Metric Type", $_POST['redistributedefaultroute_metrictype'], 1, 2);

	/* referencebandwidth (1-4294967) */
	frr_validate_intrange_input("Reference Bandwidth", $_POST['referencebandwidth'], 1, 4294967);
	/* max_metric_startup (5-86400) */
	frr_validate_intrange_input("Max Metric Seconds at Startup", $_POST['max_metric_startup'], 5, 86400);
	/* max_metric_shutdown (5-86400) */
	frr_validate_intrange_input("Max Metric Seconds at Shutdown", $_POST['max_metric_shutdown'], 5, 86400);
	/* writemultiplier (1-100) */
	frr_validate_intrange_input("Write Multiplier", $_POST['writemultiplier'], 1, 100);

}

function frr_ospf6_interfaces_validate_input() {
	global $config, $g, $input_errors;
}

function frr_ospf6_validate_input() {
	global $config, $g, $input_errors;

	if (!empty($_POST['routerid']) && !is_ipaddr($_POST['routerid'])) {
		$input_errors[] = "Router ID must be an IP address.";
	}

	if (empty($_POST['routerid'])) {

		if (!is_array($config['installedpackages']['frr']['config']) ||
		    empty($config['installedpackages']['frr']['config'][0]['routerid'])) {
			$input_errors[] = "Router ID is required because the global Router ID is not set.";
		}
	}
}


function frr_bgp_validate_input() {
	global $config, $g, $input_errors;

	if (!empty($_POST['asnum']) && !is_numeric($_POST['asnum'])) {
		$input_errors[] = "AS must be entered as a number only.";
	}

	if (!empty($_POST['routerid']) && !is_ipaddr($_POST['routerid'])) {
		$input_errors[] = "Router ID must be an IP address.";
	}

	if (!empty($_POST['holdtime']) && !is_numeric($_POST['holdtime'])) {
		$input_errors[] = "Holdtime must be entered as a number.";
	}

	if (!empty($_POST['listenip']) && !is_ipaddr($_POST['listenip'])) {
		$input_errors[] = "Listen IP must be an IP address or blank to bind to all IP addresses.";
	}
}

function frr_bgp_advanced_validate_input() {
	global $config, $input_errors;
}
function frr_bgp_neighbors_validate_input() {
	global $config, $input_errors;
}
function frr_bgp_aspaths_validate_input() {
	global $config, $input_errors, $pkg;
	frr_sort_rows_by_seq();
}
function frr_bgp_communities_validate_input() {
	global $config, $input_errors, $pkg;
	frr_sort_rows_by_seq();
}

function frr_bgp_validate_group() {
	global $config, $g, $id, $input_errors;

	if (!is_numeric($_POST['remoteas'])) {
		$input_errors[] = "Remote AS must be entered as a number only.";
	}

	if ($_POST['name'] == "") {
		$input_errors[] = "A name is required.";
	}
}
