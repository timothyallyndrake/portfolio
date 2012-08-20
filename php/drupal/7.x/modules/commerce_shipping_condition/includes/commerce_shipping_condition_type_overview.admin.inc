<?php

function commerce_shipping_condition_type_overview_form($form, &$form_state, $entity_type) {
  commerce_shipping_condition_type_overview_form_setup();
  $result = commerce_shipping_condition_type_overview_form_query();

  $rows = array();
  foreach ($result as $key => $record) {
    if (!$key) {
      break;
    }

    $cells = &$rows[];
    $cells[] = theme('menu_admin_overview', array('title' => t($record->name), 'description' => t($record->description)));
    $cells[] = (($record->status) ? t('Active') : t('Inactive'));
    $cells[] = l(t('Edit'), 'admin/commerce/config/shipping/conditions/types/manage/' . $record->type . '/edit');
    $cells[] = l(t('Clone'), 'admin/commerce/config/shipping/conditions/types/manage/' . $record->type . '/clone');
    $cells[] = l(t('Delete'), 'admin/commerce/config/shipping/conditions/types/manage/' . $record->type . '/delete');
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => array(
      array('data' => t('Type'), 'width' => '40%'),
      t('Status'),
      array('data' => t('Operations'), 'colspan' => 3),
    ),
    '#rows' => $rows,
    '#sticky' => TRUE,
    '#empty' => t('There are no shipping condition types.'),
  );

  return $form;
}

function commerce_shipping_condition_type_overview_form_query() {
  $statement = db_select('commerce_shipping_condition_type', 'et');
  $statement->fields('et', array('id', 'type', 'name', 'description', 'status'));
  $statement->orderBy('et.name');

  $connection = $statement->execute();

  $result = $connection->fetchAllAssoc('id');
  return $result;
}

function commerce_shipping_condition_type_overview_form_setup() {
  drupal_set_title('Shipping conditions');

  drupal_set_breadcrumb(array(
    l('Home', '<front>'),
    l('Administration', 'admin'),
    l('Store', 'admin/commerce'),
    l('Configuration', 'admin/commerce/config'),
    l('Shipping', 'admin/commerce/config/shipping'),
  ));
}
