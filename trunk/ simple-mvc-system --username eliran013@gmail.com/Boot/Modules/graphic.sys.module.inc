<?php 
/**
 * @name graphic.sys.module.inc
 * @description 
 * @date 29/03/2011
 * @author Eliran Pe'er
 * @team 
 */

class Graphic {
	
	/**
	 * Holds the data ($_view and $_layout) that sent from the controller.
	 */
	public $controller_data = array();
	
	/**
	 * The path to the view file.
	 */
	public $view_file = '';
	
	/**
	 * The controller's name.
	 */
	public $controller = '';
	
	/**
	 * The action's name.
	 */
	public $action = '';
	
	/**
	 * Put the controller data in $controller_data.
	 */
	public function __construct($controller_data, $view_file, $controller, $action) {
		$this->controller_data = $controller_data;
		$this->view_file = $view_file;
		$this->controller = $controller;
		$this->action = $action;
	}
	
	/**
	 * Boots the view, 
	 * if the view file does not exists, and the config allow it, it'll create a new view file.
	 */
	public function boot_view() {
		$view = $this->controller_data;
		if (is_file($this->view_file)) {
			include($this->view_file);
			return true;
		}
		else if ((bool) GRAPHIC_ALLOW_AUTO_VIEW_FILE_CREATION === true) {
			if ($this->create_view_file()) {
				include($this->view_file);
			}
		}
		return false;
	}
	
	/**
	 * Creates a new view file.
	 */
	public function create_view_file() {
		$handle = fopen($this->view_file, 'w');
		fclose($handle);
		return $handle;
	}
	
	public function link() {
		$params = func_get_args();
		
		if (substr(SERVER_DOMAIN, 0, count(SERVER_PROTOCOL)) != SERVER_PROTOCOL) {
			$link = SERVER_PROTOCOL . '://';
		}
		else {
			$link  = '';
		}
		
		$link .= SERVER_DOMAIN;
		
		if (substr(SERVER_DOMAIN, -1) != '/') { 
			$link .= '/';
		}
		
		foreach ($params as $param) {
			if (is_array($param)) {
				$link .= $param[0] . '=' . $param[1] . '/';
			}
			else {
				$link .= $param . '/';
			}
		}
		
		return $link;
	}
	
	/**
	 * Returns a formatted string, via sprintf().
	 */
	public function loop($data_array=array(), $format='', $max=null) {
		$output = '';
		$i = 0;
		foreach ($data_array as $data) {
			if (!is_null($max) && $i == $max) {
				break;
			}
			$output .= @sprintf_array($format, $data);
			$i++;
		}
		return $output;
	}
	
	/**
	 * Returns a formatted string, via a custom function.
	 */
	public function loop_function($data_array=array(), $function='', $max=null) {
		$output = '';
		$i = 0;
		foreach ($data_array as $data) {
			$data['num'] = $i;
			$data['max'] = $max;
			if (!is_null($max) && $i == $max) {
				break;
			}
			$output = call_user_func($function, $data);
		}
		return $output;
	}

}
?>