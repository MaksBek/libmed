<?php
/*
 * file 
 * Template for clinic detail view
 */
?>
<div>
    <?php if(isset($data)) { ?>
        <?php if(isset($edit_link)) { ?>
        <a href="/clinic/<?php echo $data['id'] ?>/edit">Редактировать</a>
        <?php } ?>
        <h2><?php echo $data['name'] ?></h2>
        <p>Город: <?php echo $data['city'] ?></p>
        <p>Адрес: <?php echo $data['address'] ?></p>
        <p>Телефон: <?php echo $data['phone'] ?></p>
        <?php if(!empty($data['link'])) { ?>
        <p>Сайт: <a href="<?php echo $data['link'] ?>" target="_blank"><?php echo $data['link'] ?></a></p>
        <?php } ?>
        <p>О клинике:</p>
        <?php echo html_entity_decode($data['details'], ENT_QUOTES, 'UTF-8'); ?>
    <?php } ?>
    <?php if(isset($add_new)) { ?>
        <a href="/clinic/add">Добавить клинику</a>
    <?php } ?>
</div>