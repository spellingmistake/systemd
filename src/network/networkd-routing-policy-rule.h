/* SPDX-License-Identifier: LGPL-2.1+ */
#pragma once

#include <inttypes.h>
#include <linux/fib_rules.h>
#include <stdbool.h>
#include <stdio.h>

#include "conf-parser.h"
#include "in-addr-util.h"
#include "networkd-util.h"
#include "set.h"

typedef struct Network Network;
typedef struct Link Link;
typedef struct Manager Manager;

typedef struct RoutingPolicyRule {
        Manager *manager;
        Network *network;
        Link *link;
        NetworkConfigSection *section;

        bool invert_rule;

        uint8_t tos;
        uint8_t protocol;

        uint32_t table;
        uint32_t fwmark;
        uint32_t fwmask;
        uint32_t priority;

        AddressFamily address_family; /* Specified by Family= */
        int family; /* Automatically determined by From= or To= */
        unsigned char to_prefixlen;
        unsigned char from_prefixlen;

        char *iif;
        char *oif;

        union in_addr_union to;
        union in_addr_union from;

        struct fib_rule_port_range sport;
        struct fib_rule_port_range dport;
        struct fib_rule_uid_range uid_range;

        int suppress_prefixlen;
} RoutingPolicyRule;

int routing_policy_rule_new(RoutingPolicyRule **ret);
RoutingPolicyRule *routing_policy_rule_free(RoutingPolicyRule *rule);

DEFINE_NETWORK_SECTION_FUNCTIONS(RoutingPolicyRule, routing_policy_rule_free);
int routing_policy_rule_section_verify(RoutingPolicyRule *rule);

int link_set_routing_policy_rules(Link *link);

int routing_policy_rule_add_foreign(Manager *m, RoutingPolicyRule *rule, RoutingPolicyRule **ret);
int routing_policy_rule_get(Manager *m, RoutingPolicyRule *rule, RoutingPolicyRule **ret);
int routing_policy_serialize_rules(Set *rules, FILE *f);
int routing_policy_load_rules(const char *state_file, Set **rules);

CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_tos);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_table);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_fwmark_mask);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_prefix);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_priority);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_device);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_port_range);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_ip_protocol);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_invert);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_family);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_uid_range);
CONFIG_PARSER_PROTOTYPE(config_parse_routing_policy_rule_suppress_prefixlen);
