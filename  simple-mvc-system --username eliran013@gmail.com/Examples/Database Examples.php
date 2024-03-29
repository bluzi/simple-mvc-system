<?php 	
	/**
	 * Connect to database
	 */
	$db = new Database();

	/**
	 * Select all rows from table users
	 */
	$rows = $db->table('users')->select();
	
	/**
	 * Select all rows from table users, where user_id = 1, or user id = 5
	 */
	$rows = $db
			->table('users')
			->where(null, 'user_id', '=', 1)
			->where('or', 'user_id', '=', 5)
			-select();
			
	/**
	 * Insert new row to table quz, foo = hello, and bar = world
	 */
		$db
		->table('quz')
		->fields('foo', 'bar')
		->values('hello', 'world')
		->insert();
		
	/**
	 * Delete a row from table quz where foo = hello and bar = world
	 */
		$db
		->table('quz')
		->where('', 'foo', '=', 'hello')
		->where('and', 'bar', '=', 'world')
		->delete();
?>