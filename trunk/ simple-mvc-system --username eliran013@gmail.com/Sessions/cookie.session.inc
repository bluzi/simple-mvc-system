<?php 
/**
 * @name cookie.session.inc.php
 * @description 
 * @date 04/04/2011
 * @author Eliran Pe'er
 * @team 
 */

/**
 * @see Boot/SystemInterfaces/DatabaseInterface
 */
class Session {
	
	/**
	 * Holds the default expire time of the cookie.
	 */
	const default_expire = '2592000'; // One month.
	
	/**
	 * Holds the default path of the cookie.
	 */
	const default_path = '/';
	
	/**
	 * The path of the cookie.
	 */
	protected $_path;
	
	/**
	 * The expire time of the cookie.
	 */
	protected $_expire;
	
	/**
	 * Holds all the cookies, old and new.
	 */	
	public static $cookies = array();
	
	public function __construct($path=self::default_path, $expire=self::default_expire) {
		$this->_path = $path;
		$this->_expire = $expire;
	}
	
	public static function manage_requested_data(&$cookies) {
		if (!is_array($cookies) || count($cookies) == 0) {
			return false;
		}
		self::$cookies = $cookies;
		unset ($cookies);
	}
	
	public function __get($member) {
		if (isset(self::$cookies[$member])) {
			return self::$cookies[$member];
		}
		else {
			var_dump(self::$cookies);
		}
		return false;
	}
	
	public function __set($member, $value) {
		self::$cookies[$member] = $this->_escape($value);
	}
	
	public function save() {
		foreach (self::$cookies as $member => $value) {
			setcookie($member, $value, time()+$this->_expire, $this->_path);
		}
		return true;
	}
	
	/**
	 * Delete a cookie.
	 * @param $member string
	 * Cookie's name.
	 * 
	 * @return boolean
	 */
	public function delete($member) {
		return setcookie($member, '', time()-3600, $this->_path);
	}
	
	/**
	 * Delete all cookies.
	 * @return boolean
	 */
	public function delete_all() {
		foreach (self::$cookies as $member => $value) {
			$this->Delete($member);
		}
	}
	
	/**
	 * Escape a value.
	 * @param $value string
	 * A value to escape.
	 * 
	 * @return unknown_type
	 * Escaped value.
	 */
	protected function _escape($value) {
		return $value;
	}
	
}

?>