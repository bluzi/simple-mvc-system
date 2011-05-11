<?php 
/**
 * @name database.sys.interface.inc
 * @description 
 * @date 29/03/2011
 * @author Eliran Pe'er
 * @team 
 */

interface Database {
	
	/**
	 * The constructor should contain the database connection functions, and basically connect to the database.
	 * @param $server string
	 * The connection server.
	 * 
	 * @param $username
	 * The connection username.
	 * 
	 * @param $password
	 * The connection password.
	 * 
	 * @param $database
	 * The connection database.
	 * 
	 * @return boolean
	 */
	public function __construct($server, $username, $password, $database);
	
	/**
	 * The query function should contain the database query function,
	 * so the system can use database functions that are not built in the system. (Alter, Describe, etc...)
	 * @param $q string
	 * The given query, might be an SQL query, MongoDB query or whatever.
	 */
	public function query($q);
	
	/**
	 * The insert function should take care of inserting data to the database.
	 */
	public function insert($data);
	
	/**
	 * The select function should take care of pulling data off the database.
	 * It might return an object, string, array or whatever.
	 */
	public function select($data);
	
	/**
	 * The update function should take care of updating rows in the database.
	 * It might also ALTER tables, but it's totally optional, 
	 * because built-in system modules use the query function to alter tables.
	 */
	public function update($data);
	
	/**
	 * The delete function should take care of deleteing data from the database, and everything attached to it.
	 * It might also identify importent data, and back it up.
	 */
	public function delete($data);
	
	/**
	 * The destructor is the last function running, so obviously, it should close the connection to the database.
	 */
	public function __destruct();
	
}

?>