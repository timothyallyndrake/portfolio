<?php

function commerce_shipping_condition_type_form($form, &$form_state, $entity, $op = 'edit') {
  commerce_shipping_condition_type_form_setup();

  if ($op == 'clone') {
    $entity->id = 0;
    $entity->type = '';
    $entity->name .= ' (Clone)';
  }

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $entity->id,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#default_value' => $entity->name,
    '#description' => t('The human-readable name of this shipping condition type. This text will be displayed as part of the list on the <em>Add a new shipping condition</em> page. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#maxlength' => 100,
    '#size' => 50,
    '#title' => t('Name'),
  );

  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => $entity->type,
    '#description' => t('A unique machine-readable name for this shipping condition type. It must only contain lowercase letters, numbers, and underscores. This name will be used for constructing the URL of the <em>Add a new shipping condition</em> page, in which underscores will be converted into hyphens.'),
    '#disabled' => ((empty($entity->type)) ? FALSE : TRUE),
    '#machine_name' => array(
      'exists' => 'commerce_shipping_condition_type_exists',
    ),
    '#maxlength' => 32,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => $entity->description,
    '#description' => t('Describe this shipping condition type. The text will be displayed on the <em>Add a new shipping condition</em> page.'),
    '#required' => TRUE,
    '#title' => t('Description'),
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#default_value' => $entity->status,
    '#description' => t('This will immediately enable all shipping conditions associated with this shipping condition type for inclusion into the shipping rate calculation during the checkout process.'),
    '#title' => t('Activate shipping condition type'),
  );

  $form_state['commerce_shipping_condition_type'] = $entity;
  field_attach_form('commerce_shipping_condition_type', $entity, $form, $form_state);

  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save shipping condition type'),
    '#submit' => $submit + array('commerce_shipping_condition_type_form_submit'),
  );

  if (!empty($entity->type) && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete shipping condition type'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/config/shipping/conditions/types'),
      '#submit' => $submit + array('commerce_shipping_condition_type_form_submit_delete'),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'commerce_shipping_condition_type_form_validate_custom';

  return $form;
}

function commerce_shipping_condition_type_form_setup() {
  drupal_set_breadcrumb(array(
    l('Home', '<front>'),
    l('Administration', 'admin'),
    l('Store', 'admin/commerce'),
    l('Configuration', 'admin/commerce/config'),
    l('Shipping', 'admin/commerce/config/shipping'),
    l('Shipping conditions', 'admin/commerce/config/shipping/conditions'),
  ));
}

function commerce_shipping_condition_type_form_submit($form, &$form_state) {
  $entity = entity_ui_form_submit_build_entity($form, $form_state);
  $entity->save();

  $form_state['redirect'] = 'admin/commerce/config/shipping/conditions/types';
}

function commerce_shipping_condition_type_form_submit_delete($form, &$form_state) {
  $entity = entity_ui_form_submit_build_entity($form, $form_state);

  $form_state['redirect'] = 'admin/commerce/config/shipping/conditions/types/manage/' . $entity->type . '/delete';
}

function commerce_shipping_condition_type_form_validate_custom($form, &$form_state) {
  $values = &$form_state['values'];

  if (strlen($values['description']) > 128) {
    form_set_error('description', 'Description field is limited to 128 characters.');
  }
}
