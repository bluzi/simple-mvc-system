<?php 
/**
 * @name functions.inc
 * @description 
 * @date 05/04/2011
 * @author Eliran Pe'er
 * @team 
 */

/**
 * sprintf() with an array instead of variables. (sprintf($format, array('foo', 'bar')), instead of sprintf($format, 'foo', 'bar').
 */
function sprintf_array($format, $arr) {
   return call_user_func_array('sprintf', array_merge((array) $format, $arr));
}
?>