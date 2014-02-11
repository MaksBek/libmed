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
//        $userData = db_select("users", "u")
//                ->leftJoin('users_roles', 'ur', 'u.uid = ur.uid')
//                ->condition('u.uid', (int)$uid)
//                ->fields('u')
//                ->fields('ur', array('rid'))
//                ->execute()
//                ->fetchAssoc();
        $sql = "SELECT u.*, ur.rid, fm.uri FROM users AS u "
                . "LEFT JOIN users_roles AS ur ON u.uid = ur.uid "
                . "LEFT JOIN file_managed AS fm ON u.picture = fm.fid "
                . "WHERE u.uid = " . (int)$uid;

        $userData = db_query($sql)->fetchAssoc(); 
    }
    
//    var_dump($userData); 
//    exit();
    
    
    $form = array();
    $form['#attributes'] = array(
        'enctype' => 'multipart/form-data',
        'autocomplete' => 'off'
    );
    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Логин:'),
        '#size' => '30',
        '#default_value' => isset($userData) ? $userData['name'] : ""
    );
    $form['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email:'),
        '#size' => '30',
        '#default_value' => isset($userData) ? $userData['mail'] : ""
    );
    $form['password'] = array(
        '#type' => 'password',
        '#title' => t('Пароль:'),
        '#size' => '30'
    );
    $form['role'] = array(
        '#type' => 'select',
        '#title' => t('Выберите один из вариантов'),
        '#options' => array(
            '4' => t('Администратор'),
            '5' => t('Врачь')
        ),
        '#default_value' => isset($userData) ? $userData['rid'] : 4
    );
    $form['fullname'] = array(
        '#type' => 'textfield',
        '#title' => t('Имя врача/администратора:'),
        '#size' => '30',
        '#default_value' => isset($userData) ? $userData['fio'] : ""
    );
    $form['specialization'] = array(
        '#type' => 'textfield',
        '#title' => t('Специальность'),
        '#size' => '30',
        '#default_value' => isset($userData) ? $userData['specialization'] : ""
    );
    
    $form['about'] = array(
        '#type' => 'textarea',
        '#title' => t('Детальная информация о враче:'),
        '#cols' => 40,
        '#rows' => 7,
        '#default_value' => isset($userData) ? $userData['about'] : ''
    );
    
    $form['picture_upload'] = array(
        '#type' => 'file',
        '#title' => t('Фотография пользователя:')
    );
    $form['uid'] = array(
        '#type' => 'hidden',
        '#default_value' => isset($userData) ? $userData['uid'] : 0
    );
    if(isset($userData) && isset($userData['fid'])){
        $fid = $userData['fid'];
    } else {
        $fid = 0;
    }
    $form['picture'] = array(
        '#type' => 'hidden',
        '#value' =>  $fid
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Сохранить')
    );
    
    
    $form['#submit'][] = 'clinic_register_handler';
    
    return $form;
}

function clinic_personal_form_validate($form, $form_state){
    echo "VALIDATION <PRE>";
    $data = $form_state['values'];
    if($data['uid'] == 0 && __checkUserName($data['name'])){
        echo 'ok';
    }
    
    //var_dump($form_state); exit();
}

function clinic_register_handler($form, $form_state){
    global $user;
    $validators = array(
        'file_validate_is_image' => array(),
    );
    $file = file_save_upload('picture_upload', $validators);

    if ($file == false) {
//        form_set_error('picture_upload', t('Failed to upload picture'));
        $picture = null;
    } elseif ($file != null) {
        $picture = $file;
        // Set the user id
//        $picture->uid = $user->uid;
    }
    
    if(!is_null($picture)){
        $fid = $picture->fid;
    } else {
        $fid = 0;
    }
    
    $data = $form_state['values'];
    
    if(!empty($data['password'])){
        require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
        $data['password'] = user_hash_password(trim($data['password']));
    } else {
        $data['password'] = "";
    }
    
    if($data['uid'] > 0){
//        var_dump($data); exit();
        $res = db_update("users")
                ->fields(array(
                'name' => $data['name'],
                'mail' => $data['email'],
                'picture' => $fid,
                'init' => $data['email'],
                'fio' => $data['fullname'],
                'specialization' => $data['specialization'],
                'about' => $data['about']
        ))
                ->condition('uid', (int)$data['uid'])
                ->execute();
        
        $result = db_update("users_roles")
                ->fields(array('rid' => $data['role']))
                ->condition('uid', $data['uid'], '=')
                ->execute();
        drupal_set_message("Даные пользователя успешно обновлены");
        drupal_goto('user/' . $user->uid . '/personal');
    } else {
        $next_id = db_next_id(db_query('SELECT MAX(uid) FROM {users}')->fetchField());
        $uid = db_insert("users")
            ->fields(array(
                'uid' => $next_id,
                'name' => $data['name'],
                'mail' => $data['email'],
                'pass' => $data['password'],
                'created' => mktime(),
                'status' => 1,
                'language' => 'ru',
                'picture' => $fid,
                'init' => $data['email'],
                'fio' => $data['fullname'],
                'specialization' => $data['specialization'],
                'about' => $data['about']
            ))
            ->execute();
        db_insert('users_roles')
            ->fields(array(
                'uid' => $next_id,
                'rid' => $data['role']
            ))
            ->execute();
        $res = db_query("SELECT clinic_id FROM clinic_to_user WHERE uid = " . (int)$user->uid);
        $clinic = $res->fetchObject();
        
        db_insert('clinic_to_user')
            ->fields(array(
                'clinic_id' => $clinic->clinic_id,
                'uid' => $next_id
            ))
            ->execute();
        
        drupal_set_message("Пользователь успешно добавлен");
        drupal_goto('user/' . $user->uid . '/personal');
    }
    
    var_dump($picture);exit();
}
function __checkUserName($name){
    $result = db_select('users', 'u')
            ->fields('u')
            ->condition('u.name', $name)
            ->execute()
            ->fetchAllAssoc('uid');
    if(count($result)){
        return true;
    } else {
        return false;
    }
}
