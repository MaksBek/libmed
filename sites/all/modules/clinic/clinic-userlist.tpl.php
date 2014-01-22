<?php
 //var_dump($users);
 echo '<a href="/personal/add">[Добавить сотрудника]</a>';
if(count($users)){
    echo '<ul>'; 
    foreach($users as $user){
        if(isset($user->image) && $user->image != NULL){
            $image = '<img src="' . file_create_url($user->image) . '" width="100" />';
        } else {
            $image = '<img src="/sites/default/files/pictures/no-image-blog.png" width="100" />';
        }
        echo '<li>' . $image . ' ' . $user->name . '<span style="float:right"><a href="/personal/' . $user->uid . '/edit">[Редактировать]</a></span></li>';
    }
    echo '</ul>';
}