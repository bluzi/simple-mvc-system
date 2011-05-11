<?php 
/**
 * @name controller.sys.module.inc
 * @description 
 * @date 29/03/2011
 * @author Eliran Pe'er
 * @team 
 */

class Controller {
	
	/**
	 * The view array, that will be sent to the view page.
	 * @var object
	 */
	protected $_view;
	
	/**
	 * Defines the layout options, this array is send from the controller to the layout module.
	 * @var object
	 */
	protected $_layout;
	
	/**
	 * The function that called when the controller dies.
	 */
	public function kill() {
	}
	
	/**
	 * The function that called when the controller starts.
	 */
	public function init() {
	}
	
	public function getData() {
		return $this->_view;
	}
	
	protected function __get($member) {
	}
	
	protected function __set($member, $value) {
	}
	
	/**
	 * @param $controller string
	 * The controller name.
	 * 
	 * @param $action string
	 * The current action name.
	 */
	final public function __construct() {
		$this->init();
	}
	
	/*public function IndexAction() {
		var_dump(get_class_methods($_GET['SYSTEM']['Controller']));
		exit();
	}*/
	
	/**
	 * The destructor loads the view page and starts the 
	 */
	final public function __destruct() {
		$this->kill();
	}
	
}

?>