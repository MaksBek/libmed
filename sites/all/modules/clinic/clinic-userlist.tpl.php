<?php
// var_dump($users);
 echo '<a href="/personal/add">[Добавить сотрудника]</a>';
if(count($users)){ ?>
    <ul>
    <?php foreach($users as $user){
        if(isset($user->image) && $user->image != NULL){
            $image = '<img src="' . file_create_url($user->image) . '" width="100" />';
        } else {
            $image = '<img src="/sites/default/files/pictures/No_Photo_Available.jpg" width="100" />';
        } ?>
        <li>
            <table>
                <tr>
                    <td width="100"><?php echo $image ?></td>
                    <td valign="top"><?php echo $user->fio; ?></td>
                    <td valign="top">
                        <?php if(!empty($user->specialization)) { ?>Специальность: <?php echo $user->specialization . '<br />'; } ?>
                        <?php if(!empty($user->about)){echo "О Враче:<br />" . $user->about;} ?>
                    </td>
                    <td>
                        <span style="float:right"><a href="/personal/<?php echo $user->uid; ?>/edit">[Редактировать]</a></span>
                    </td>
                </tr>
            </table>
        </li>
<?php } ?>
    </ul>
<?php } ?>}