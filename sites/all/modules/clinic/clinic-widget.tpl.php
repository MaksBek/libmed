<?php

/* 
 * Template for list of clinics
 */
// var_dump($clinics);
if(count($clinics)){
foreach($clinics as $clinic) { ?>
<div>
    <h3><a href="/clinic/<?php echo $clinic->id ?>/view"><?php echo $clinic->name; ?></a></h3>
    <div>
        <?php echo (strlen($clinic->details) > 150) ? mb_strcut($clinic->details, 0, 150, 'utf-8') . '..' : html_entity_decode($clinic->details, ENT_QUOTES, 'utf-8'); ?>
        <br>
        <a href="/clinic/<?php echo $clinic->id ?>/view">Детальнее</a>
    </div>
</div>
<?php } ?>
<?php } else { ?>
<p>Данные отсуцтвуют</p>
<?php } ?>
