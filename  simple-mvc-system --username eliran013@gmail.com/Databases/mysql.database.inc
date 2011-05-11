<?php 
/**
 * @name mysql.database.inc
 * @description 
 * @date 29/03/2011
 * @author Eliran Pe'er
 * @team 
 */

class Database {
	
	// Constants
	/* Database connection information */
	const default_server = 'loaclhost';
	const default_username = 'pozeint1_schoolp';
	const default_password = 'a1b2c3';
	const default_database = 'pozeint1_schoolpro';

	/* Action names */
	const action_query = 'query';
	const action_query_text = 'query_text';
	const action_special = 'special';
	const action_errors = 'errors';
	
	/* Default return values */
	const method_deafult_return = self::action_query;
	
	/* Default variable values */
	const fields_default_value = '*';
	const order_by_deafult_value = 'ASC';
	const default_where_condition = 'AND';

	
	// Class variables
	/**
	 * Holds the databaes connection.
	 */
	protected $_connection;
	
	/**
	 * Holds the query variables (table, fields, values, where and etc...)
	 */
	protected $_parts = array();
	
	/**
	 * Holds the last method information 
	 * (SQL values, success/failed and etc, but not the query results, such as SELECT results. 
	 * They will be returned automatically.)
	 */
	public $last_action_info = array();
	
	/**
	 * Holds the database connection information. (server, username, password and database)
	 */
	protected $_db_info = array();
	
	// "_parts" variable functions, or, the functions that defines the query variables. (table, fields, where and etc...)
	
	/**
	 * Define the "from" variable.
	 * Example: $db->from('users')->..
	 */
	public function from($table) {
		$this->_parts['table'] = (string) '`' . $this->_escape($table) . '`';
		return $this;
	}
	
	/**
	 * The same as from(), but with different name.
	 */
	public function table($table) { return $this->from($table); }
	
	/**
	 * Define the "fields" (or columns) variable, in insert, select and so on...
	 * Example: $db->from('users')->fields('foo', 'bar');
	 */
	public function fields() {
		$arr = $this->_parts['fields_array'] = func_get_args();
		if (func_num_args() == 1 && $arr[0] == '*' || func_get_args() == 0) {
			$cols = '*';
		}
		else {
			$cols = '';
			$first = true;
			foreach ($arr as $col) {
				if (!$first) {
					$cols .= ', ';
				}
				$cols .= '`' . $col . '`';
				$first = false;
			}
		}
		
		$this->_parts['fields'] = (string) $cols;
		return $this;
	}
	
	/**
	 * Define the "fields" (or columns) variable, the only differnce between fields() is that the parameter is an array.
	 * Example: 
	 * $fields = array('username', 'password');
	 * $db->from('users')->fields_array($fields)->select();
	 */
	public function fields_array($arr) {
		if (!is_array($arr)) { return false; }
		$this->_parts['fields_array'] = $arr;
		
		$cols = '';
		$first = true;
		foreach ($arr as $col) {
			if (!$first) {
				$cols .= ', ';
			}
			$cols .= '`' . $col . '`';
			$first = false;
		}
		
		$this->_parts['fields'] = (string) $cols;
		return $this;
	}
	
	/**
	 * Define the "distinct" variable.
	 * If set to true, the database won't return duplicate values.
	 * The default value is false. 
	 * Example: $db->from('foo')->distinct()->select();
	 */
	public function distinct($value=true) {
		$this->_parts['distinct'] = (bool) $value;
		return $this;
	}
	
	/**
	 * Define the "temporary" variable, most likely for a CREATE TABLE (create()) query.
	 * The default value is false. 
	 * Example: $db->table('foo')->temporary()->create();
	 */
	public function temporary($value=true) {
		$this->_parts['temporary'] = (bool) $value;
		return $this;
	}
	
	/**
	 * Define the "exists" variable, most likely for a CREATE TABLE (create()) query.
	 * The default value is false. 
	 * Example: $db->table('foo')->exists(false)->create();
	 * Will make CREATE TABLE `foo` IF NOT EXISTS
	 * with is the same as: $db->table('foo')->exists()->create();
	 */
	public function exists($value=false) {
		$this->_parts['exists'] = (bool) $value;
		return $this;
	}
	
	/**
	 * Define the "values" array. (in insert, update and so on...)
	 * Example: $db->from('users')->fields('foo', 'bar')->values('quz', 'baz');
	 */
	public function values() {
		$arr = $this->_parts['values_array'] = func_get_args();
		$cols = '';
		$first = true;
		foreach ($arr as $col) {
			if (is_string($col)) {
				$col = '"' . $this->_escape($col) . '"';
			}
			if (!$first) {
				$cols .= ', ';
			}
			$cols .= $col;
			$first = false;
		}
		
		$this->_parts['values'] = (string) $cols;
		return $this;
	}
	
	/**
	 * Define the "values" array, the only differnce between fields() is that the parameter is an array.
	 * Example: 
	 * $values = array('foo', 'bar');
	 * $db->from('users')->fields('username', 'password')->values_array($values)->insert();
	 * 
	 * Will insert foo to username, and bar to password.
	*/
	public function values_array($arr) {
		if (!is_array($arr)) { return false; }
		$arr = array_values($arr);
		$this->_parts['values_array'] = $arr;
		
		$cols = '';
		$first = true;
		foreach ($arr as $col) {
			if (is_string($col)) {
				$col = '"' . $this->_escape($col) . '"';
			}
			if (!$first) {
				$cols .= ', ';
			}
			$cols .= $col;
			$first = false;
		}
		
		$this->_parts['values'] = (string) $cols;
		return $this;
	}
	
	/**
	 * Define the "where" array. (in almost anything... select, update and etc...)
	 * Example: $db->from('foo')->where(null, 'bar' '=', '1')->where('or', 'quz', '>', 100)->...
	 */
	public function where($condition=null, $field, $equation, $value) {
		if (is_null($condition) || strtolower($condition) != 'or' && strtolower($condition) != 'and') {
			$condition = self::default_where_condition;
		}
		if (is_string($value)) {
			$value = (string) '"' . $this->_escape($value) . '"';
		}
		if (is_array($this->_parts['where']) && count($this->_parts['where']) > 0) {
			$this->_parts['where'][] = (string) strtoupper($condition) . ' ' . $this->_escape($field) . ' ' . $equation . ' ' . $value;
		}
		else {
			$this->_parts['where'][] = (string) $this->_escape($field) . ' ' . $equation . ' ' . $value;
		}
		
		return $this;
	}
	
	/**
	 * Define the "limit" value, mostly in SELECT queries.
	 * Examples:
	 * $db->...->limit(14,32)->...
	 * $db->...->limit(50)->...
	 */
	public function limit($count, $offset=null) {
		if (is_null($offset)) {
			$this->_parts['limit'] = (string) (int) $count;
		}
		else {
			$this->_parts['limit'] = (string) (int) $count . ', ' . (int) $offset;
		}
		return $this;
	}
	
	/**
	 * Define the "order" (or ORDER BY) value, mostly in SELECT queries.
	 * Example: 
	 * $db->from('foo')
	 * ->where(null, 'bar', '>', 10)
	 * ->order(array(
	 * 'foo' => 'desc',
	 * 'bar' => 'asc',
	 * ))->...
	 */
	public function order($arr) {
		if (!is_array($arr)) {
			return false;
		}
		
		$order = '';
		$first = true;
		foreach ($arr as $key => $value) {
			if (empty($value) || is_null($value) || strtolower($value) != 'asc' && strtolower($value) != 'desc') {
				$value = self::order_by_deafult_value;
			}
			if (!$first) {
				$order .= ' ,';
			}
			$order .= (string) $key . ' ' . strtoupper($value);
			$first = false;
		}
		$this->_parts['order'] = (string) $order;
		return $this;
	}
	
	// System functions
	/**
	 * For each database method (insert, select and etc...), this method sets the required variables.
	 * For example, when you set _required_variables('table', 'fields'), and the developer didn't defined "table" or "fields",
	 * it'll print an error message.
	 */
	protected function _required_variables() {
		$variables = func_get_args();
		foreach ($variables as $var) {
			$var = strtolower($var);
			if (!isset($this->_parts[$var])) {
				return '<strong>Database Error:</strong> Please set "' . $var . '".';
			}
		}
		return true;
	}
	
	/**
	 * This method makes a WHERE query out of a useless "where" array in the system.
	 */
	protected function _fetch_where() {
		if (isset($this->_parts['where']) && is_array($this->_parts['where'])) {
			return $this->_parts['fetched_where'] = ' WHERE ' . implode(' ', $this->_parts['where']);
		}
		return false;
	}
	
	/**
	 * This method turns the values and fields arrays, into a UPDATE-query formatted string.
	 */
	protected function _fetch_values() {
		if (isset($this->_parts['values_array']) && is_array($this->_parts['values_array']) && isset($this->_parts['fields_array']) && is_array($this->_parts['fields_array'])) {
			$i = 0;
			$this->_parts['fetched_values'] = '';
			while ($i < count($this->_parts['fields_array'])) {
				if (is_string($this->_parts['values_array'][$i])) {
					$value = '"' . $this->_escape($this->_parts['values_array'][$i]) . '"';
				}
				else {
					$value = $this->_parts['values_array'][$i];
				}
				
				if ($i != 0) {
					$this->_parts['fetched_values'] .= ', ';
				}
				
				$this->_parts['fetched_values'] .= '`' . $this->_parts['fields_array'][$i] . '` = ' . $value;
				$i++;
			}
		}
		else {
			return false;
		}
		return $this->_parts['fetched_values'];
	}
	
	/**
	 * Adds a custom parameter to the "_parts" array. 
	 * This is not recommanded.
	 */
	public function add_custom_param($key, $value) {
		$this->_part[$key] = $value;
	}
	
	/**
	 * Sends a return output for databaes functions without results. (insert, delete and etc...)
	 * It'll also update the last_action_info array, and fill it with usefull information.
	 */
	protected function _output($query=null, $sql=null, $special=null) {
		$this->last_action_info = array(
			self::action_query => $query,
			self::action_query_text => $sql,
			self::action_special => $special,
			self::action_errors => mysql_error(),
		);
		
		return $this->last_action_info[self::method_deafult_return];
	}
	
	/**
	 * Clear the _parts array. It's required to use this method in the end of any database method.
	 */
	protected function _clear_cache() {
		$this->_parts = array();
	}
	
	/**
	 * A public method, to clear all the cache information. (the _prats array, and the last_action_info array)
	 * Use it wisely.
	 */
	public function clear_all_cache() {
		_clear_cache();
		$this->last_action_info = array();
	}
	
	/**
	 * This method fill all the required but missing variables, with deafult values.
	 */
	protected function _fill_empty_variables() {
		if (!isset($this->_parts['fields']) || empty($this->_parts['fields'])) {
			$this->_parts['fields'] = self::fields_default_value;
		}
	}
	
	/**
	 * Fill the "db_info" array with connection information. (server, username, password and database.)
	 */
	public function fill_connection_information($server, $username, $password, $database) {
		$this->_db_info = array(
			'server' => $server,
			'username' => $username,
			'password' => $password,
			'database' => $database,
		);
		return true;
	}
	
	// MySQL Functions
	/**
	 * Define the "db_info" array, and fill it with connection information.
	 */
	public function __construct($server=self::default_server, $username=self::default_username, $password=self::default_password, $database=self::default_database, $connect=true) {	
		$this->fill_connection_information($server, $username, $password, $database);
			
		if ((bool) $connect === true) {
			return $this->connect();
		}
		else {
			return false;
		}
	}
	
	/**
	 * Kill the database connection when the db object dies.
	 */
	public function __destruct() {
		$this->close_connection();
	}
	
	/**
	 * Make the database connection. Don't call it before you fill the "db_info" array!
	 */
	public function connect() {
		if (!$this->_connection = mysql_connect($this->_db_info['serevr'], $this->_db_info['username'], $this->_db_info['password'])) {
			return false;
		}
		if (!mysql_select_db($this->_db_info['database'])) {
			return false;
		}
		return true;
	}
	
	/**
	 * Close the old connection, and creates a new one.
	 */
	public function switch_connection($server, $username, $password, $database) {
		$this->close_connection();
		
		$this->fill_connection_information($server, $username, $password, $database);
		
		$this->_connection = null;
		
		$this->connect();
	}
	
	/**
	 * Kills a database connection.
	 */	
	public function close_connection() {
		return false;
	}
	
	/**
	 * Escape a string.
	 */
	protected function _escape($string) {
		return mysql_real_escape_string($string);
	}
	
	/**
	 * Run a custom query.
	 * Not recommended to use outside the object, but still possible.
	 */
	public function query($q) {
		return mysql_query($q);
	}
	
	/**
	 * Fetch the results sent from a DB query, most likely from a SELECT query.
	 */
	public function fetch_query($q) {
		if ($q === false) {
			return false;
		}
		else {
			if (@mysql_num_rows($q) == 0) {
				$output = array();
			}
			else if (@mysql_num_rows($q) == 1) {
				$output = array(mysql_fetch_assoc($q));
			}
			else {
				$output = mysql_fetch_array($q);
			}
		}
		
		@mysql_free_result($q);
		return $output;
	}
	
	// Real database methods.
	/**
	 * Insert a row to the database.
	 * MySQL definition: "INSERT inserts new rows into an existing table."
	 */
	public function insert($run=true) {		
		$this->_required_variables('table', 'fields', 'values');
		
		$sql = 'INSERT INTO ' . $this->_parts['table'] . ' (' . $this->_parts['fields'] . ') VALUES (' . $this->_parts['values'] . ');';
		
		if ((bool) $run) {
			$query = $this->query($sql);
		}
		
		$this->_clear_cache();
		return $this->_output($query, $sql, mysql_insert_id());
	}
	
	/**
	 * Update a row from the database.
	 * MySQL definition: "For the single-table syntax, the UPDATE statement updates columns of existing rows in the named table with new values."
	 */
	public function update($return_type=0, $run=true) {
		$this->_required_variables('table', 'fields', 'values', 'where');
		
		$sql = 'UPDATE ' . $this->_parts['table'] . ' SET ' . $this->_fetch_values()	. $this->_fetch_where() . ';';
		
		if ((bool) $run) {
			$query = $this->query($sql);
		}
		
		$this->_clear_cache();
		return $this->_output($query, $sql, mysql_affected_rows());
	}
	
	/**
	 * Select rows from the database, and fetch them automatically.
	 * MySQL definition: "SELECT is used to retrieve rows selected from one or more tables."
	 */
	public function select($run=true) {
		$this->_required_variables('table');
		$this->_fill_empty_variables();
		
		$sql = 'SELECT ';
		if (isset($this->_parts['distinct']) && $this->_parts['distinct'] === true) { $sql .= 'DISTINCT '; }
		$sql .= $this->_parts['fields'] . ' FROM ' . $this->_parts['table'];
		if (isset($this->_parts['where'])) { $sql .= $this->_fetch_where(); }
		if (isset($this->_parts['limit'])) { $sql .= ' LIMIT ' . $this->_parts['limit']; }
		if (isset($this->_parts['order'])) { $sql .= ' ORDER BY ' . $this->_parts['order']; }
		$sql .= ';';
		
		if ((bool) $run) {
			$query = $this->query($sql);
		}
		
		$this->_clear_cache();
		$this->_output($query, $sql, @mysql_num_rows());
		
		if ((bool) $run && $query !== false) {
			return $this->fetch_query($query);
		}
		else {
			return false;
		}
		
	}
	
	/**
	 * Delete rows from the database.
	 * MySQL definition: "For the single-table syntax, the DELETE statement deletes rows from a table and returns a count of the number of deleted rows."
	 */
	public function delete($run=true) {
		$this->_required_variables('table', 'where');
		
		$sql = 'DELETE FROM ' . $this->_parts['table'] . $this->_fetch_where();
		
		if ((bool) $run) {
			$query = $this->query($sql);
		}
		
		$this->_clear_cache();
		return $this->_output($query, $sql, mysql_affected_rows());
	}
	
	/**
	 * Describe a table.
	 * MySQL definition: "DESCRIBE provides information about the columns in a table."
	 */
	public function describe($run=true) {
		$this->_required_variables('table');
		
		$sql = 'DESCRIBE ' . $this->_parts['table'] . ';';
		
		if ((bool) $run) {
			$query = $this->query($sql);
		}
		
		$this->_clear_cache();
		$this->_output($query, $sql);
		
		if ((bool) $run && $query !== false) {
			while ($r = mysql_fetch_assoc($query)) {
				$output[$r['Field']] = $r;
			}
			return $output;
		}
	}
	
	public function create($run=true) {
		$this->_required_variables('table');
		
		$sql = 'CREATE ';
		if (isset($this->_parts['temporary']) && (bool) $this->_parts['temporary'] === true) { $sql .= 'TEMPORARY '; }
		$sql .= 'TABLE ';
		if (isset($this->_parts['exists']) && (bool) $this->_parts['exists'] === false) { $sql .= 'IF NOT EXISTS'; }
		$sql .= $this->_parts['table'];
		
		if ((bool) $run) {
			$query = $this->query($sql);
		}
		
		$this->_clear_cache();
		return $this->_output($query, $sql);
	}
	
	public function alter($run=true) {
	}
	
}

?>