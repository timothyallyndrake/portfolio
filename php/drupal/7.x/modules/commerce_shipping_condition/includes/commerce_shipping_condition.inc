<?php

class CommerceShippingCondition extends Entity {

  public $id;
  public $type;
  public $name;
  public $description;
  public $status;
  public $commerce_flat_rate_name;
  public $commerce_product_type;
  public $country_code;
  public $rate;

  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_shipping_condition');
  }

  protected function defaultLabel() {
    return $this->name;
  }

}

class CommerceShippingConditionController extends EntityAPIController {

  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  public function create(array $values = array()) {
    $types = array_keys(commerce_shipping_condition_type_options_list());

    $values += array( 
      'id' => 0,
      'type' => ((count($types)) ? $types[0] : ''),
      'name' => '',
      'description' => '',
      'status' => 0,
      'commerce_flat_rate_name' => '',
      'commerce_product_type' => '',
      'country_code' => '',
      'rate' => 0,
    );

    $entity = parent::create($values);

    return $entity;
  }

}

class CommerceShippingConditionUIController extends EntityDefaultUIController {

  public function hook_menu() {
    $items = array();

    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;
    $wildcard_position = count(explode('/', $this->path));

    $items[$this->path] = array(
      'title' => $this->entityInfo['plural label'],
      'description' => 'Manage ' . strtolower($this->entityInfo['plural label']) . '.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_overview_form', $this->entityType),
      'access callback' => 'commerce_shipping_condition_access',
      'file path' => drupal_get_path('module', $this->entityInfo['module']) . '/includes',
      'file' => $this->entityType . '_overview.admin.inc',
      'type' => MENU_LOCAL_TASK,
      'weight' => 6,
    );

    $items[$this->path . '/list'] = array(
      'title' => $this->entityInfo['plural label'],
      'description' => 'Manage ' . $this->entityInfo['plural label'] . '.',
      'access callback' => 'commerce_shipping_condition_access',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -8,
    );

    $items[$this->path . '/add'] = array(
      'title' => 'Add ' . strtolower($this->entityInfo['label']),
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType),
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, NULL, 'add'),
      'access callback' => 'commerce_shipping_condition_access',
      'file path' => drupal_get_path('module', $this->entityInfo['module']) . '/includes',
      'file' => $this->entityType . '.admin.inc',
      'type' => MENU_LOCAL_ACTION,
    );

    $items[$this->path . '/manage/' . $wildcard] = array(
      'title' => 'Edit',
      'title callback' => 'entity_label',
      'title arguments' => array($this->entityType, ($wildcard_position + 1)),
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, ($wildcard_position + 1)),
      'load arguments' => array($this->entityType),
      'access callback' => 'commerce_shipping_condition_access',
      'file path' => drupal_get_path('module', $this->entityInfo['module']) . '/includes',
      'file' => $this->entityType . '.admin.inc',
    );

    $items[$this->path . '/manage/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'load arguments' => array($this->entityType),
      'access callback' => 'commerce_shipping_condition_access',
      'type' => MENU_DEFAULT_LOCAL_TASK,
    );

    $items[$this->path . '/manage/' . $wildcard . '/clone'] = array(
      'title' => 'Clone',
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, ($wildcard_position + 1), 'clone'),
      'load arguments' => array($this->entityType),
      'access callback' => 'commerce_shipping_condition_access',
      'file path' => drupal_get_path('module', $this->entityInfo['module']) . '/includes',
      'file' => $this->entityType . '.admin.inc',
    );

    $items[$this->path . '/manage/' . $wildcard . '/%'] = array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_operation_form', $this->entityType, ($wildcard_position + 1), ($wildcard_position + 2)),
      'load arguments' => array($this->entityType),
      'access callback' => 'commerce_shipping_condition_access',
      'file' => 'includes/entity.ui.inc',
    );

    return $items;
  }

}
