<?php 
/**
 * @name index.php
 * @description 
 * @date 29/03/2011
 * @author Eliran Pe'er
 * @team 
 */

	/**
	 * __autoload is a magic method that calls everytime a new object created.
	 */
	
	function __autoload($class) {
		if (strpos($class, 'Module') != false) { // Check if it's a module.
			$class = str_replace('Module', '', $class);
			include_once(FOLDER_MODULES . $class . '.module.inc');
		}
	}

	// Include all the config files.
	$folder = '../Config/';
	$handle = opendir($folder);
	while (false !== ($file = readdir($handle))) {
        if (is_file($folder . $file)) {
        	include_once($folder . $file);
        }
    }
    
    // Include all the system modules.
    $folder = FOLDER_SYSTEM_MODULES;
	$handle = opendir($folder);
	while (false !== ($file = readdir($handle))) {
        if (is_file($folder . $file)) {
        	include_once($folder . $file);
        }
    }
    
    // Include the database class.
    include_once('../Databases/' . DATABASE_TYPE . '.database.inc');
    
    // Include the session class.
    include_once('../Sessions/' . SESSION_TYPE . '.session.inc');
    
    // Manage post data.
    Form::manage_post_data($_POST);
    
    // Manage session data
    Session::manage_requested_data($_COOKIE);
	
	if ($_GET['request'] == 'controller') {
		if (isset($_GET['path']) && strpos($_GET['path'], '/') !== false) {
			// Orginaize variables, and fetch the URL
			$path = explode('/', $_GET['path']);
			$controller = $path[0];
			$action = $path[1];
			unset($path[0], $path[1]);
			$variables = $path;
			
			if (empty($controller) || empty($action)) {
				exit('<strong>Fatel Error:</strong> Cannot fetch URL.');
			}
			
			// Fetch get variables
			$_GET = array();
			foreach ($variables as $var) {
				if (!empty($var)) {
					list ($key, $value) = explode('=', $var);
					if (is_null($value) || $value == 'true') {
						$value = true;
					}
					else if (empty($value) || $value == 'false') {
						$value = false;
					}
					$_GET[$key] = $value;
				}
			}
									
			// Include the controller
			$controller_file = FOLDER_CONTROLLERS . $controller . '.controller.inc';
			if (is_file($controller_file)) {
				include_once($controller_file);
				$controller_class = $controller . 'Controller';
				$controller_object = new $controller_class();
				call_user_func(array($controller_object, $action . 'Action'));
				
				// Include the layout, and the view.
				$view_file = FOLDER_VIEWS . $controller . '.' . $action . '.inc';
				$graphic = new Graphic($controller_object->getData(), $view_file, $controller, $action);
				include_once(FOLDER_LAYOUT . 'index.inc');
			}
			else {
				exit('<strong>Fatel Error:</strong> Cannot find controller.');
			}
		}
		else {
			exit('<strong>Fatel Error:</strong> Failed to fetch URL.');
		}
	}
	else {
		exit('<strong>Fatel Error:</strong> Unknown request.');
	}
	
?>