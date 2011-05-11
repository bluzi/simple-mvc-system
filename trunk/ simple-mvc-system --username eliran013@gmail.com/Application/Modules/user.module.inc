<?php 
/**
 * @name student.module.inc
 * @description 
 * @date 31/03/2011
 * @author Tomer Shvadron
 * @team Eliran Pe'er
 */

/**
 * This is an example module.
 */

class userModule {
		
	// Constants
	
	/**
	 * Holds the column name in the database for this object's Id.
	 */
	const id = 'user_id';
	
	/**
	 * The data table in the database.
	 */
	const table = '_users';

	// Variables

	/**
	 * The user type
	 */
	public $user_type = '';
	
	/**
	 * The user ID
	 */
	public $user_id = '';

	// Methods
	
	public function __construct($id) {
		$db = new Database();
		$user = $db
		        ->table(self::table)
		        ->fields('type')
		        ->where(null, self::id, '=', $id)
		        ->select();
		
		if ($user === false) {
			return false;
		}
		        
		$this->user_type = $user[0]['type'];
		$this->user_id = $id;
		
		return true;
	}
	
	public function object() {
		if ($this->user_type == 'student') {
			return new studentModule((int) $this->user_id);
		}
		else if ($this->user_type == 'teacher') {
			return new teacherModule((int) $this->user_id);
		}
		return false;
	}
	
	public static function login($user_id, $password) {
		$session = new Session();
		$session->user_id = $user_id;
		$session->password = $password;
		$session->save();
	}
	
	public static function is_logged_in() {
		$session = new Session();
		$db = new Database();
		$user = $db
			->table(self::table)
			->fields(self::id)
			->where('', self::id, '=', $session->user_id)
			->where('and', 'password', '=', $session->password)
			->select();
		
		if ($user === false) {
			return false;
		}
		
		return true;
	}

}
?>