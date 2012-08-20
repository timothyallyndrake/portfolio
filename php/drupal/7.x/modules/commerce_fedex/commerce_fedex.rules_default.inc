<?php

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_fedex_default_rules_configuration() {
  $rules = array();

  $rule = rule(commerce_fedex_component_variables());
  $rule->label = t('Create FedEx shipments');
  $rule->action(
    'commerce_fedex_shipment_create_by_order',
    array(
      'commerce_order:select' => 'commerce-order',
    )
  );

  $rules['commerce_fedex_shipment_create_by_order'] = $rule;

  return $rules;
}

function commerce_fedex_component_variables() {
  return array(
    'commerce_order' => array(
      'type' => 'commerce_order',
      'label' => t('Order'),
    ),
  );
}
