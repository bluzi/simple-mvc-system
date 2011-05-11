<?php 
/**
 * This is an example view.
 */
	if (isset($view->error)) {
		echo $view->error;
		echo '<br />';
	}
	echo $view->login_form;
?>