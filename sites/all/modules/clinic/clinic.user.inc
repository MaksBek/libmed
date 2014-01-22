<?php

function clinic_personal_view($user){
    $cl = db_select("clinic_to_user", 'c2u')
            ->fields("c2u", array('clinic_id'))
            ->condition('c2u.uid', $user->uid)
            ->execute()
            ->fetchAssoc();

    $result = db_query("SELECT DISTINCT u.*, (SELECT fm.uri FROM file_managed fm WHERE u.picture = fm.fid) AS image FROM `users` AS u "
            . " LEFT JOIN `users_roles` AS ur ON u.uid = ur.uid"
            . " LEFT JOIN `clinic_to_user` AS c2u ON u.uid = c2u.uid"
            . " WHERE c2u.clinic_id = " . (int)$cl['clinic_id'] . " AND ur.rid IN (5,6)"
        );
       
    return theme('clinic_user_list', array('users' => $result->fetchAll()));
}

function clinic_personal_form($form, $form_state, $uid = 0){
    if($uid > 0) {
        $userData = db_select("users", "u")
                ->fields('u')
                ->condition('u.uid', (int)$uid)
                ->execute()
                ->fetchAssoc();
    }
    
    $form = array();
    
    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Логин:'),
        '#size' => '30',
        '#default_value' => isset($userData) ? $userData['name'] : ""
    );
    
     
}
