<?php 
/**
 * @name form.sys.module.inc
 * @description 
 * @date 29/03/2011
 * @author Eliran Pe'er
 * @team 
 */

class Form {
	
	/**
	 * Constants
	 */
	const default_input_type = 'text';
	const static_form_default_action = '';
	const static_form_default_method = 'post';
	const form_default_id = '';
	const form_default_class = '';
	const reserved_attributes = 'label';
	
	const info_action = 'action';
	const info_method = 'method';
	const info_id = 'id';
	const info_class = 'class';
	
	const validators_array = 'email,in_array,numeric,string_length_less,string_length_greater,string_length_equal,zip_code,between,date,alphanumeric,alpha,url,ip,not_empty,regex,less_than,greater_than,equal,function'; // Delimited with ','.
	const filters_array = 'boolean,encrypt,htmlspecialchars,strip_tags,int,lower,upper,strip_newline,nl2br,function,regex'; // Delimited with ','.
	const types_array = 'text,password,radio,checkbox,submit'; // Delimited with ','.
	
	/**
	 * Holds all the <input> elements.
	 */
	protected $_elements = array();
	
	/**
	 * Holds all the attributes for <input> tags. (name, class, style and so on...)
	 */
	protected $_attrs = array();
	
	/**
	 * Holds the <option> elements.
	 */
	protected $_options = array();
	
	/**
	 * Holds the blocks and their names.
	 */
	protected $_blocks = array();
	
	/**
	 * Holds the form info: Action, method, class and ID.
	 */
	protected $_form_info;
	
	/**
	 * A string contains HTML of the last element created.
	 */
	public $last_element = '';
	
	/**
	 * After a post request is sent, this array holds the post data.
	 */
	public static $requested_data = array();
	
	/**
	 * After a post request is sent, this array holds the validator errors.
	 */
	public static $errors = array();

	/**
	 * An array of validators.
	 */
	protected $_validators = array();
	
	/**
	 * An array of filters.
	 */
	protected $_filters = array();
	
	/**
	 * The values that came from fill_values().
	 */
	protected $_values = array();
	
	/**
	 * Sets the form information (action, method, id and class) when the object is created.
	 */
	public function __construct($action=self::static_form_default_action, $method=self::static_form_default_method, $id=self::form_default_id, $class=self::form_default_class) {
		$this->_form_info = array(
			self::info_action => $action,
			self::info_method => $method,
			self::info_id => $id,
			self::info_class => $class,
		);
		return true;
	}
	
	/**
	 * Remove the post variable, and put it in the $requested_data array instead.
	 */
	public static function manage_post_data(&$post) {
		if (count($post) > 0) {
			foreach ($post as $key => $value) {
				$key = Encryption::decrypt($key);	
				$name = self::fetch_name($key);
				$errors[$name] = self::is_valid_request($key, $value);
				$output[$name] = self::apply_filters($key, $value);
			}
			self::$requested_data = $output;
			self::$errors = $errors;
		}
		else {
			self::$requested_data = false;
		}
		$post = array();
	}
	
	/**
	 * Check if there are any validation errors in the current request.
	 */
	public static function is_erroneous() {
		if (count(self::$errors) !== 0) {
			return false;
		}
		return true;
	}
	
	/**
	 * Returns the number of errors according to an element.
	 */
	public static function num_errors($name) {
		if (!is_array(self::$errors[$name])) {
			return false;
		}
		
		return count(self::$errors[$name]);
	}
	
	protected function _encrypt_name_attr($name, $validators, $filters) {
		$output = '!name="' . $name . '"';
		foreach ($validators as $key => $value) {
			$output .= '#' . $key . '="' . $value . '"';
		}
		
		foreach ($filters as $key => $value) {
			$output .= '%' . $key . '="' . $value . '"';
		}
		
		return Encryption::encrypt($output);
	}
	
	public function fill_values($values=array()) {
		foreach ($values as $element_name => $value) {
			$this->_values[$element_name] = $value;
		}
	}
	
	/**
	 * Returns HTML of an element. The attributes are fechted by the attr() method.
	 */
	public function element($type=self::default_input_type) {
		if ((bool) $this->_is_valid_type($type) === false) {
			return false;
		}
		if ($type != 'submit') {
			if (!isset($this->_attrs['name']) || empty($this->_attrs['name'])) {
				return false;
			}
		}
		
		if (isset($this->_attrs['name']) && !isset($this->_attrs['value']) && isset($this->_values[$this->_attrs['name']])) {
			$this->_attrs['value'] = $this->_values[$this->_attrs['name']];
		}
		
		if (isset($this->_attrs['name'])) {
			$this->_attrs['name'] = $this->_encrypt_name_attr($this->_attrs['name'], $this->_validators, $this->_filters);
		}
		
		$output = '';
		
		// Add the label in the begainning on the element
		if (isset($this->_attrs['label'])) { $output .= $this->_attrs['label']; }
		
		// Fetch the type
		if ($type == 'select') { // Select
			$output .= '<select';
				if ($this->_num_attrs($this->_attrs) > 0) { $output .= ' '; }
				$output .= $this->_render_attrs($this->_attrs);
			$output .= '>' . "\n";
				foreach ($this->_options as $option) {
					$output .= $option;
				}
			$output .= '</select>' . "\n";
		}
		else { // Other input (text, submit, etc...)
			$output .= '<input type="' . $type . '" ';
				$output .= $this->_render_attrs($this->_attrs);
			$output .= '/>' . "\n";
		}
		
		if (!isset($this->_attr['name'])) {
			$this->_elements[] = $output;
		}
		/*else if (array_key_exists($this->_elements, $this->_attr['name'])) {
			return false;
		}*/
		else {
			$this->_elements[$this->_attr['name']] = $output;
		}
		$this->last_element = $output;
		$this->_clear_cache();
		return $output;
	}
	
	/**
	 * After a POST request sent, this method fetch the name attribute out of the encrypted string.
	 */
	public static function fetch_name($name) {
		preg_match('/\!name\=\"(.*?)\"/', $name, $output);
		return $output[1];
	}
	
	/**
	 * After a PSOT request sent, this method checks if a value is valid, according to the validators in the element.
	 */
	public static function is_valid_request($validators, $data) {
		preg_match_all('/\#(.*?)\=\"(.*?)\"/', $validators, $validators_array);
		if (count($validators_array[1]) == 0 || count($validators_array[2]) == 0) {
			return array();
		}
		$validators = array_combine($validators_array[1], $validators_array[2]);
		$allowed_validators = explode(',', self::validators_array);
		$errors = array();
		
		foreach ($validators as $validator => $value) {
			if (in_array($validator, $allowed_validators)) { 
				switch ($validator) {
					case 'email':
						if ((bool) self::validate_email($data) === false) {
							$errors[] = $validator;
						}
					break;
					case 'numeric':
						if (!is_numeric($data)) {
							$errors[] = $validator;
						}
					break;
					case 'string_length_less':
						if (strlen($data) > $value) {
							$errors[] = $validator;
						}
					break;
					case 'string_length_greater':
						if (strlen($data) < $value) {
							$errors[] = $validator;
						}
					break;
					case 'string_length_equal':
						if (strlen($data) != $value) {
							$errors[] = $validator;
						}
					break;
					case 'zip_code':
						if ((bool) self::check_zip_code($data) === false) {
							$errors[] = $validator;
						}
					break;
					case 'between':
						if (strpos($value, ',') === false) { return false; }
						$value = explode(',', $value);
						if (is_numeric($data) && (float) $data > (float) $value[0] && (float) $data < (float) $value[1]) {
							// True
						}
						else {
							$errors[] = $validator;
						}
					break;
					case 'date':
						if ((bool) self::check_date($data, $value) === false) {
							$errors[] = $validator;
						}
					break;
					case 'alphanumeric':
						if (!ctype_alnum($data)) {
							$errors[] = $validator;
						}
					break;
					case 'alpha':
						if (ereg('[^A-Za-z]', $text)) { 
							$errors[] = $validator;
						}
					break;
					case 'url':
						if (!preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $url)) {
							$errors[] = $validator;
						}
					break;
					case 'ip':
						if ((bool) self::check_ip_address($data) === false) {
							$errors[] = $validator;
						}
					break;
					case 'not_empty':
						if (empty($data)) {
							$errors[] = $validator;
						}
					break;
					case 'regex':
						if (!preg_match($value, $data)) {
							$errors[] = $validator;
						}
					break;
					case 'less_than':
						if ($data > $value) {
							$errors[] = $validator;
						}
					break;
					case 'greater_than':
						if ($data < $value) {
							$errors[] = $validator;
						}
					break;
					case 'equal':
						if ($data != $value) {
							$errors[] = $validator;
						}
					break;
					case 'function':
						if ((bool) call_user_func($value, $data) === false) {
							$errors[] = $validator;
						}
					break;
				}
			}
		}
		return $errors;
	}
	
	/**
	 * After a POST request sent, this method apply the element's filters on it's value.
	 */	
	public static function apply_filters($filters, $data) {
		// 'boolean,encrypt,htmlspecialchars,strip_tags,int,lower,upper,strip_newlines,nl2br,function_reference,regex,trim'
		preg_match_all('/\%(.*?)\=\"(.*?)\"/', $filters, $filters_array);
		if (count($filters_array[1]) == 0 || count($filters_array[2]) == 0) {
			return $data;
		}
		$filters = array_combine($filters_array[1], $filters_array[2]);
		$allowed_filters = explode(',', self::filters_array);
		foreach ($filters as $filter => $value) {
			if (in_array($filter, $allowed_filters)) { 
				switch ($filter) {
					case 'boolean':
						$data = (boolean) $data;
					break;
					case 'encrypt':
						$data = Encryption::encrypt($data);
					break;
					case 'htmlspecialchars':
						$data = htmlspecialchars($data);
					break;
					case 'strip_tags':
						$data = strip_tags($data, $value);
					break;
					case 'int':
						$data = (int) $data;
					break;
					case 'lower':
						$data = strtolower($data);
					break;
					case 'upper':
						$data = strtoupper($data);
					break;
					case 'strip_newlines':
						$data = str_replace(array('<br>', '<br />', '<br/>'), '', nl2br($data));
					break;
					case 'nl2br':
						$data = nl2br($data);
					break;
					case 'function':
						$data = call_user_func($value, $data);
					break;
					case 'function_reference':
						call_user_func($value, $data);
					break;
					case 'regex':
						
					break;
					case 'trim':
						$data = trim($data);
					break;
				}
			}
		}
		return $data;
	}
	
	/**
	 * Checks if an email is valid.
	 */
	public static function validate_email($email) {
	  // First, we check that there's one @ symbol, 
	  // and that the lengths are right.
	  if (!ereg("^[^@]{1,64}@[^@]{1,255}$", $email)) {
	    // Email invalid because wrong number of characters 
	    // in one section or wrong number of @ symbols.
	    return false;
	  }
	  // Split it into sections to make life easier
	  $email_array = explode("@", $email);
	  $local_array = explode(".", $email_array[0]);
	  for ($i = 0; $i < sizeof($local_array); $i++) {
	    if
	(!ereg("^(([A-Za-z0-9!#$%&'*+/=?^_`{|}~-][A-Za-z0-9!#$%&
	↪'*+/=?^_`{|}~\.-]{0,63})|(\"[^(\\|\")]{0,62}\"))$",
	$local_array[$i])) {
	      return false;
	    }
	  }
	  // Check if domain is IP. If not, 
	  // it should be valid domain name
	  if (!ereg("^\[?[0-9\.]+\]?$", $email_array[1])) {
	    $domain_array = explode(".", $email_array[1]);
	    if (sizeof($domain_array) < 2) {
	        return false; // Not enough parts to domain
	    }
	    for ($i = 0; $i < sizeof($domain_array); $i++) {
	      if
	(!ereg("^(([A-Za-z0-9][A-Za-z0-9-]{0,61}[A-Za-z0-9])|
	↪([A-Za-z0-9]+))$",
	$domain_array[$i])) {
	        return false;
	      }
	    }
	  }
	  return true;
	}
	
	/**
	 * Check if a zip code is valid.
	 */
	public static function check_zip_code($zip_code) {
		if(preg_match("/^([0-9]{5})(-[0-9]{4})?$/i",$zip_code)) {
			return true;
		}
		else {
    		return false;
		}
	}
	
	/** 
	 * Checks date if matches given format and validity of the date. 
	 * Examples: 
	 * <code> 
	 * check_date('22.22.2222', 'mm.dd.yyyy'); // returns false 
	 * check_date('11/30/2008', 'mm/dd/yyyy'); // returns true 
	 * check_date('30-01-2008', 'dd-mm-yyyy'); // returns true 
	 * check_date('2008 01 30', 'yyyy mm dd'); // returns true 
	 * </code> 
	 * @param string $value the variable being evaluated. 
	 * @param string $format Format of the date. Any combination of <i>mm<i>, <i>dd<i>, <i>yyyy<i> 
	 * with single character separator between. 
	 */ 
	public static function check_date($value, $format='dd.mm.yyyy') {
		if (strlen($value) >= 6 && strlen($format) == 10) { 
	        // find separator. Remove all other characters from $format 
	        $separator_only = str_replace(array('m','d','y'),'', $format); 
	        $separator = $separator_only[0]; // separator is first character 
	        
	        if ($separator && strlen($separator_only) == 2) { 
	            // make regex 
	            $regexp = str_replace('mm', '(0?[1-9]|1[0-2])', $format); 
	            $regexp = str_replace('dd', '(0?[1-9]|[1-2][0-9]|3[0-1])', $regexp); 
	            $regexp = str_replace('yyyy', '(19|20)?[0-9][0-9]', $regexp); 
	            $regexp = str_replace($separator, "\\" . $separator, $regexp); 
	            
	            if ($regexp != $value && preg_match('/'.$regexp.'\z/', $value)) { 
	                // check date 
	                $arr=explode($separator,$value); 
	                $day=$arr[0]; 
	                $month=$arr[1]; 
	                $year=$arr[2]; 
	                
	                if (@checkdate($month, $day, $year)) {
	                    return true; 
	                }
	            } 
	        } 
	    } 
	    return false; 
	} 
	
	/**
	 * Check if an IP address is valid.
	 */
	public static function check_ip_address($ip_addr) {
	  // First of all the format of the ip address is matched
	  if (preg_match("/^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/", $ip_addr)) {
	    // Now all the intger values are separated
	    $parts = explode(".", $ip_addr);
	    // Now we need to check each part can range from 0-255
	    foreach ($parts as $ip_parts)
	    {
	      if (intval($ip_parts) > 255 || intval($ip_parts) < 0)
	      return false; // If number is not within range of 0-255
	    }
	    return true;
	  }
	  else
	    return false; // If format of ip address doesn't matches
	}
	
	/**
	 * Clears the cache after a new element.
	 */
	protected function _clear_cache() {
		$this->_attrs = array();
		$this->_options = array();
	}
	
	/**
	 * Get an element, if $key is null, it'll return the last element.
	 */
	public function get($key=null) {
		if (is_null($key) && !empty($this->last_element)) {
			return $this->last_element;
		}
		else if (array_key_exists($key, $this->_elements)) {
			return $this->_elements[$key];
		}
		return false;
	}
	
	/**
	 * Returns HTML of an AJAX form.
	 */
	public function setAsAjaxForm() {
	}
	
	/**
	 * Returns HTML of a normal form.
	 */
	public function setAsStaticForm() {
		return $this->form_tag(implode('<br />', $this->_elements));
	}
	
	/**
	 * Returns HTML made of custom blocks.
	 */
	public function setAsBlockSetForm() {
		if (func_num_args() < 2) { return false; }
		$block_names = func_get_args();
		$format = $block_names[0];
		$format .= "\n";
		unset($block_names[0]);
		
		foreach ($block_names as $name) {
			if (array_key_exists($name, $this->_blocks)) {
				$blocks[] = implode('', $this->_blocks[$name]);
			}
		}
		
		if (is_array($blocks)) {
			if (count($block_names) != count($blocks)) { return false; }
			
			$output = @sprintf_array($format, $blocks);
			
			if (!$output) { return false; }
			
			return $this->form_tag($output);
		}
		else {
			return false;
		}
	}
	
	/**
	 * Add new elements block.
	 */
	public function set_block() {
		if (func_num_args() < 2) { return false; }
		$elements = func_get_args();
		$name = $elements[0];
		unset($elements[0]);
				
		$this->_blocks[$name] = $elements;
		return $this;
	}
	
	/**
	 * Returns HTML of a <form> and </form> tags. The $elements variable is an array with HTML of the inputs inside the form.
	 */
	public function form_tag($elements) {
		$output = '<form action="' . $this->_form_info[self::info_action] . '" method="' . $this->_form_info[self::info_method] . '" id="' . $this->_form_info[self::info_id] . '" class="' . $this->_form_info[self::info_class] . '">' . "\n";
			$output .= $elements;
		$output .= '</form>' . "\n";
		
		return $output;
	}
	
	/**
	 * Check if a type attribute is valid. 
	 */
	protected function _is_valid_type($type) {
		$types_array = explode(',', self::types_array);
		if (in_array($type, $types_array)) {
			return true;
		}
		else {
			return false;
		}
	}
	
	/**
	 * Render all the attributes from $_attr to HTML format. (key="value")
	 */
	protected function _render_attrs($attrs) {
	$output = '';
	if ($this->_num_attrs($attrs) < 1) { return false; }
		$reserved_attributes = explode(', ', self::reserved_attributes);
		foreach($attrs as $key => $value) {
			if (is_array($reserved_attributes) && !in_array($key, $reserved_attributes) || $key == $reserved_attributes) {
				$output .= $key . '="' . $value . '" ';
			}
		}
		$output = substr($output, 0, -1);
		
		return $output;
	}
	
	/**
	 * Returns the number of the attributes in $_attr as an integer.
	 */
	protected function _num_attrs($attrs) {
		if (!is_array($attrs)) { return 0; }
		return (int) count($attrs);
	}
	
	/**
	 * Add an attribute.
	 * Example: $form->attr('id', 'myElement')->element('text');
	 */
	public function attr($key, $value) {
		$this->_attrs[$key] = $value;
		return $this;
	}
	
	/**
	 * Sets the label for an element. $format is a (s)printf formatted string. 
	 * (for example, $format = '<div>%s:</div>')
	 */
	public function label($value, $format=null) {
		if (is_null($format)) {
			$this->_attrs['label'] = $value;
		}
		else {
			$label = @sprintf($format, $value);
			if ($label === false) {
				return false;
			}
			$this->_attrs['label'] = $label;
		}
		
		return $this;
	}
	
	public function validator($name, $value='') {
		$this->_validators[$name] = $value;
		return $this;
	}
	
	public function filter($name, $value='') {
		$this->_filters[$name] = $value;
		return $this;
	}
	
	/**
	 * Add an option element to the $_options array. 
	 * Most likely for a <select> list.
	 */
	public function option($value, $attrs=array()) {
		$option = "\t" . '<option';
			if ($this->_num_attrs($attrs) > 0) { $option .= ' '; }
			$option .= $this->_render_attrs($attrs);
		$option .= '>' . $value . '</option>' . "\n";
	
		$this->_options[] = $option;
		return $this;
	}
}
?>