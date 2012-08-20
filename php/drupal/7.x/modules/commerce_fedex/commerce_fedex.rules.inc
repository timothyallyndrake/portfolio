<?php

/**
 * Implements hook_rules_action_info().
 */
function commerce_fedex_rules_action_info() {
  $actions = array();

  $actions['commerce_fedex_shipment_create_by_order'] = array(
    'label' => t('Create FedEx shipments'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce FedEx'),
  );

  return $actions;
}
