<?php 
	mysql_connect('localhost', 'pozeint1_schoolp', 'a1b2c3') or die(mysql_error());
	mysql_select_db('pozeint1_schoolpro');
	
	mysql_query('INSERT INTO `_classes` (`name`) VALUES("' . $_POST['bla'] . '");') or die(mysql_error());
?>