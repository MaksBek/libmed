<?php

function clinic_personal_view($user){
    $cl = db_select("clinic_to_user", 'c2u')
            ->fields("c2u", array('clinic_id'))
            ->condition('c2u.uid', $user->uid)
            ->execute()
            ->fetchAssoc();

    $result = db_select("users", "u")
            ->fields("u")
            ->fields('ur')
            ->leftJoin("users_roles", "ur", "u.uid = ur.uid")
            ->leftJoin("clinic_to_user", "c2u", "u.uid = c2u.uid")
            ->condition('c2u.clinic_id', $cl['clinic_id'])
            ->condition("ur.rid IN ", "(5,6)");
//            ->execute()
//            ->fetchAllAssoc();
 var_dump($cl['clinic_id']); exit();   
    return theme('clinic_user_list', array('data' => 'asdasd'));
}
