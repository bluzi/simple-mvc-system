<?php
/**
 * @name encryption.sys.module.inc
 * @description 
 * @date 04/03/2011
 * @author Outsourcing
 * @team 
 */

/**
 * Two way Encryption and Decryption Class.
 *
 * Suitable for storing credential information like credit card, bank account information into the database
 *
 * PHP version 5+
 * - require Mcrypt library installed
 *
 * @package     raynux
 * @subpackage  raynux.labs.crypt
 * @version     1.0
 * @author	Md. Rayhan Chowdhury
 * @license     MIT License http://www.opensource.org/licenses/mit-license.php
 */

/**
 * Encrypt & Decrypt Using PHP MCrypt Library
 *
 * @package     raynux
 * @subpackage  raynux.labs.crypt
 * @author      Md. Rayhan Chowdhury
 */
class Encryption {

    /**
     * RayCrypt Instances
     *
     * @var RayCrypt
     */
    protected static $_instances = array();

    /**
     * Secret Salt generated from the key
     *
     * @var string
     */
    protected $_secretKey;

    /**
     * Default Configuration Array
     *
     * @var array
     */
    protected $_defaults = array(
                                    'returnPlainText' => true,
                                    'salt' => '7$e4!158f6$%#be533.!@a066!#35428^#%&$*&faa91:!982*#!e07'
                                );

    /**
     * Hold Run-time configuration
     *
     * @var array
     */
    protected $_configs = array();

    /**
     * Class Constructor
     *
     * @param array $options
     */
    private function __construct($options = null) {
        $this->_setConfig($options);
    }

    /**
     * Get Instance of RayCrypt
     *
     * @param string $configName
     * @param array $options
     * @return RayCrypt
     */
    private static function &_getInstance($configName = null, $options = null) {

        if (empty($configName)) {
            $configName = 'default';
        }

        if (empty(self::$_instances[$configName])) {
            self::$_instances[$configName] = new self($options);
        }

        return self::$_instances[$configName];
    }

    /**
     * Configure the class instance
     *
     * @param array $options  array('salt' => '', 'returnPlainText' => true)
     * @return RayCrypt
     */
    protected function &_setConfig($options = null) {

        if (empty($options) || !is_array($options)) {
            $options = array();
        }

        $this->_configs = array_merge($this->_defaults, $options);

        if (!empty($this->_configs['salt'])) {
            $this->_setSecretKey($this->_configs['salt']);
        }

        return $this;
    }

    /**
     * Set secret salt for encryption algorithm
     * @param string $key
     * @return RayCrypt
     * @access protected
     */
    protected function &_setSecretKey($key) {
        $this->_secretKey = md5(substr(sha1($key), 0, 32));
        return $this;
    }

    /**
     *
     * @param  $configName
     * @param  $options
     */
    public static function config($configName, $options = null) {
        $_this = self::_getInstance($configName, $options);
    }

    /**
     * Encrypt using Mcrypt PHP Library
     *
     * @param string $value
     * @param string $configName
     * @return string
     * @access public
     * @static
     */
    public static function encrypt($value, $configName = null) {

        $_this = self::_getInstance($configName);
        $key = $_this->_secretKey;

        $iv_size = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB);
        $iv = mcrypt_create_iv($iv_size, MCRYPT_RAND);
        $encryped = mcrypt_encrypt(MCRYPT_RIJNDAEL_256, $key, $value, MCRYPT_MODE_ECB, $iv);
        if (!empty($_this->_configs['returnPlainText'])) {
            $encryped = base64_encode($encryped);
        }

        return $encryped;
    }

    /**
     * Decrypt Using Mcrypt
     *
     * @param string $value
     * @param string $configName
     * @return string
     * @access public
     * @static
     */
    public static function decrypt($value, $configName = null) {
        $_this = self::_getInstance($configName);
        $key = $_this->_secretKey;

        if (!empty($_this->_configs['returnPlainText'])) {
            $value = base64_decode($value);
        }

        $iv_size = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB);
        $iv = mcrypt_create_iv($iv_size, MCRYPT_RAND);
        return rtrim(mcrypt_decrypt(MCRYPT_RIJNDAEL_256, $key, $value, MCRYPT_MODE_ECB, $iv), "\0\4");
    }

    /**
     * Masks Important Information Padding String
     *
     * helpful in displaying a part of credit card information like
     * 01234567890 will result xxxxxxx7890
     *
     * @param string $string
     * @param integer $length specify negative length to mask from backward
     * @param mask character $maskCharacter
     * @return string
     * @access public
     * @static
     */
    public static function padString($string, $length, $maskCharacter = 'X') {
        $maskString = substr($string, 0, $length);
        $maskString = preg_replace('(.)', $maskCharacter, $maskString);
        $string = $maskString . substr($string, $length);
        return $string;
    }

}

?>