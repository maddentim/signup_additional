<?php
// $Id: signup_additional.inc,v 1.1.2.3 2009/09/21 06:56:40 dww Exp $

/**
 * @file
 * Code to confirm the e-mail during signups. Since this code is only needed
 * when building or submitting the signup form (or editing an existing
 * signup), it all lives in this include file and is only loaded when needed.
 */

/**
 * Alter the signup form to add the e-mail confirmation functionality.
 */
function signup_additional_alter_signup_form(&$form, &$form_state, $form_id) {
  global $user;
//  $form['form_array'] = array('#value' => '<pre>' . print_r($form) . '</pre>');
  if ($form_id == 'signup_edit_form') {
    $signup = $form['#signup'];

  
    // Ensure the user has permission to edit their own signups, or the rest
    // of this is wasted effort for a form that can't be submitted.
    $node = node_load($signup->nid);
    $admin = _signup_menu_access($node, 'admin');
    $own = !empty($user->uid) && $user->uid == $signup->uid;
    $can_edit = $admin || (user_access('edit own signups') && $own);
    if (!$can_edit) {
      return;
    }
  }

  // We only need to do this for authenticated users signing up
  // themselves. We already collect the (presumably current) e-mail for
  // "anonymous" signups. If an administrator is signing up another user,
  // there's no reason to include the e-mail confirmation field, either.

  $options = array(
    '0' => t('0'),
    '1' => t('1'),
    '2' => t('2'),
    '3' => t('3'),
    '4' => t('4'),
    '5' => t('5'),
    '6' => t('6'),
    '7' => t('7'),
    '8' => t('8'),
    '9' => t('9'),
  );
  if ($form_id == 'signup_form') {
    $form['collapse']['signup_additional'] = array(
        '#type' => 'select',
        '#title' => t('Additional signups'),
        '#description' => t('Please indicate the number of people attending in addition to yourself.'),
        '#default_value' => t('0'),
        '#weight' => 0,
        '#options' => $options,
      );
    $form['collapse']['submit']['#weight'] = 2;
    $form['#validate'][] = 'signup_additional_signup_form_validate_additional';
  }
  if ($form_id == 'signup_edit_form') {
    $signup = $form['#signup'];
    $additional = ($signup->count_towards_limit - 1);
    $form['elements']['signup_additional'] = array(
        '#type' => 'select',
        '#title' => t('Additional signups'),
        '#description' => t('Please indicate the number of people attending in addition to yourself.'),
        '#default_value' => $additional,
        '#weight' => 0,
        '#options' => $options,
      );
    $form['elements']['submit']['#weight'] = 2;
    $form['#validate'][] = 'signup_additional_signup_form_validate_additional';
  }
}
  
function signup_additional_signup_form_validate_additional($form, &$form_state) {
  if ($form_state['values']['signup_additional'] < 0 || $form_state['values']['signup_additional'] > 9 ) {
    form_set_error('signup_additional', t('You must select a number between 0 and 10.'));
  }
}

