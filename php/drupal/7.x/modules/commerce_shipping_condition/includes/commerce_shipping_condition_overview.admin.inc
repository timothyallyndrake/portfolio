<?php

function commerce_shipping_condition_overview_form($form, &$form_state, $entity_type) {
  commerce_shipping_condition_overview_form_setup();
  $result = commerce_shipping_condition_overview_form_query();

  if (empty($result)) {
    $record = new stdClass();
    $record->et_type = 'empty';
    $record->et_name = 'N/A';
    $result = array(0 => $record);
  }

  $commerce_flat_rate_names = commerce_shipping_condition_commerce_shipping_service_option_list();

  $commerce_product_types = array('' => 'Not Set');
  $commerce_product_types += commerce_product_type_options_list();

  $country_codes = array('' => 'Not Set');
  $country_codes += _addressfield_country_options_list();

  foreach ($result as $key => $record) {
    if (empty($form[$record->et_type])) {
      $form[$record->et_type]['table'] = array(
        '#theme' => 'table',
        '#header' => array(
          array('data' => t('<em>Type:</em> ' . $record->et_name), 'width' => '20%'),
          array('data' => t('Status'), 'width' => '10%'),
          array('data' => t('Shipping Service'), 'width' => '10%'),
          array('data' => t('Conditions'), 'width' => '10%'),
          array('data' => t('Rate'), 'width' => '20%'),
          array('data' => t('Operations'), 'colspan' => 3, 'width' => '30%'),
        ),
        '#rows' => array(),
        '#sticky' => TRUE,
        '#empty' => t('There are no shipping conditions.'),
      );
    }

    if (!$key) {
      break;
    }

    $rows = &$form[$record->et_type]['table']['#rows'];
    $cells = &$rows[];

    $cells[] = theme('menu_admin_overview', array('title' => t($record->e_name), 'description' => t($record->description)));
    $cells[] = theme('menu_admin_overview', array(
      'title' => (($record->e_status) ? t('Active') : t('Inactive')),
      'description' => t('<em>Type: </em>') . (($record->et_status) ? t('Active') : t('Inactive'))
    ));
    $cells[] = t($commerce_flat_rate_names[$record->e_commerce_flat_rate_name]);
    $cells[] = t('Product Type:') . ' <em>' . t($commerce_product_types[$record->e_commerce_product_type]) . '<em/><br>'
             . t('Country:') . ' <em>' . t($country_codes[$record->e_country_code]) . '<em/>';
    $cells[] = commerce_currency_format($record->e_rate, 'USD', NULL, FALSE);
    $cells[] = l(t('Edit'), 'admin/commerce/config/shipping/conditions/manage/' . $record->e_id . '/edit');
    $cells[] = l(t('Clone'), 'admin/commerce/config/shipping/conditions/manage/' . $record->e_id . '/clone');
    $cells[] = l(t('Delete'), 'admin/commerce/config/shipping/conditions/manage/' . $record->e_id . '/delete');
  }

  return $form;
}

function commerce_shipping_condition_overview_form_query() {
  $statement = db_select('commerce_shipping_condition', 'e');
  $statement->join('commerce_shipping_condition_type', 'et', 'et.type = e.type');
  $statement->addExpression('e.id', 'e_id');
  $statement->addExpression('e.type', 'e_type');
  $statement->addExpression('et.type', 'et_type');
  $statement->addExpression('e.name', 'e_name');
  $statement->addExpression('et.name', 'et_name');
  $statement->addExpression('e.rate', 'e_rate');
  $statement->addExpression('e.status', 'e_status');
  $statement->addExpression('et.status', 'et_status');
  $statement->addExpression('ifnull(e.description, et.description)', 'description');
  $statement->addExpression('e.commerce_flat_rate_name', 'e_commerce_flat_rate_name');
  $statement->addExpression("nullif(e.commerce_product_type, '')", 'e_commerce_product_type');
  $statement->addExpression("nullif(e.country_code, '')", 'e_country_code');
  $statement->orderBy('et.name');
  $statement->orderBy('e.name');

  $connection = $statement->execute();

  $result = $connection->fetchAllAssoc('e_id');
  return $result;
}

function commerce_shipping_condition_overview_form_setup() {
  drupal_set_title('Shipping conditions');

  drupal_set_breadcrumb(array(
    l('Home', '<front>'),
    l('Administration', 'admin'),
    l('Store', 'admin/commerce'),
    l('Configuration', 'admin/commerce/config'),
    l('Shipping', 'admin/commerce/config/shipping'),
  ));

  setlocale(LC_MONETARY, 'en_US');
}
