<?php

function commerce_shipping_condition_form($form, &$form_state, $entity, $op = 'edit') {
  commerce_shipping_condition_form_setup();

  if ($op == 'clone') {
    $entity->id = 0;
    $entity->name .= ' (Clone)';
  }

  $entity_types = commerce_shipping_condition_type_options_list();

  if (!count($entity_types)) {
    drupal_set_message(t('There are no shipping condition types. Please <a href="@add-entity-type">add a shipping condition type</a>.', array('@add-entity-type' => url('admin/commerce/config/shipping/conditions/types/add'))), 'warning');
    return;
  }

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $entity->id,
  );

  $form['commerce_flat_rate_name'] = array(
    '#type' => 'select',
    '#default_value' => $entity->commerce_flat_rate_name,
    '#description' => t('The shipping service of this shipping condition.'),
    '#options' => commerce_shipping_condition_commerce_shipping_service_option_list(),
    '#required' => TRUE,
    '#title' => t('Shipping Service'),
  );

  $form['type'] = array(
    '#type' => 'select',
    '#default_value' => $entity->type,
    '#description' => t('The type of this shipping condition.'),
    '#options' => $entity_types,
    '#required' => TRUE,
    '#title' => t('Type'),
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#default_value' => $entity->name,
    '#description' => t('The human-readable name of this shipping condition. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#maxlength' => 32,
    '#size' => 50,
    '#title' => t('Name'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => $entity->description,
    '#description' => t('Describe this shipping condition.'),
    '#title' => t('Description'),
  );

  $commerce_product_types = array('' => '-- Select --');
  $commerce_product_types += commerce_product_type_options_list();

  $form['commerce_product_type'] = array(
    '#type' => 'select',
    '#default_value' => $entity->commerce_product_type,
    '#description' => t('The product type for this shipping condition.'),
    '#options' => $commerce_product_types,
    '#title' => t('Product Type'),
  );

  $country_codes = array('' => '-- Select --');
  $country_codes += _addressfield_country_options_list();

  $form['country_code'] = array(
    '#type' => 'select',
    '#default_value' => $entity->country_code,
    '#description' => t('The country for this shipping condition.'),
    '#options' => $country_codes,
    '#title' => t('Country'),
  );

  $form['rate'] = array(
    '#type' => 'textfield',
    '#default_value' => $entity->rate,
    '#description' => t('The monetary rate assigned once shipping conditions are met.'),
    '#required' => TRUE,
    '#maxlength' => 10,
    '#size' => 50,
    '#title' => t('Rate <em>$</em>'),
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#default_value' => $entity->status,
    '#description' => t('This will immediately enable this shipping condition for inclusion into the shipping rate calculation during the checkout process.'),
    '#title' => t('Activate shipping condition'),
  );

  $form_state['commerce_shipping_condition'] = $entity;
  field_attach_form('commerce_shipping_condition', $entity, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save entity'),
    '#submit' => $submit + array('commerce_shipping_condition_form_submit'),
  );

  if ($entity->id && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete entity'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/config/shipping/conditions'),
      '#submit' => $submit + array('commerce_shipping_condition_form_submit_delete'),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'commerce_shipping_condition_form_validate_custom';

  return $form;
}

function commerce_shipping_condition_form_setup() {
  drupal_set_breadcrumb(array(
    l('Home', '<front>'),
    l('Administration', 'admin'),
    l('Store', 'admin/commerce'),
    l('Configuration', 'admin/commerce/config'),
    l('Shipping', 'admin/commerce/config/shipping'),
    l('Shipping conditions', 'admin/commerce/config/shipping/conditions'),
  ));
}

function commerce_shipping_condition_form_submit($form, &$form_state) {
  $values = &$form_state['values'];

  $entity = entity_ui_form_submit_build_entity($form, $form_state);
  $entity->save();

  $form_state['redirect'] = 'admin/commerce/config/shipping/conditions';
}

function commerce_shipping_condition_form_submit_delete($form, &$form_state) {
  $entity = entity_ui_form_submit_build_entity($form, $form_state);

  $form_state['redirect'] = 'admin/commerce/config/shipping/conditions/manage/' . $entity->id . '/delete';
}

function commerce_shipping_condition_form_validate_custom($form, &$form_state) {
  $values = &$form_state['values'];

  if (strlen($values['description']) > 128) {
    form_set_error('description', 'Description field is limited to 128 characters.');
  }

  if (!is_numeric($form_state['values']['rate'])) {
    form_set_error('description', 'Rate field is limited to numeric characters.');
  }
}
