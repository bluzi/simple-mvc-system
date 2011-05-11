<?php 
/**
 * This is an example controller, it's only here to show the power of the system.
 * Here, we'll implement a login page. We'll use the form module, and some modules with are not built-in, like the user module.
 */
class indexController extends Controller {

	/**
	 * The site homepage.
	 */
	public function indexAction() {
		// Code here
	}
	
	/**
	 * The site's login page.
	 */
	public function loginAction() {
		if (Form::$requested_data !== false) {
			$data = Form::$requested_data;
			$user = new userModule($data['user_id']);
			$user = $user->object();
			if ($user !== false && $user->password == $data['password']) {
				userModule::login($user->user_id, $user->password);
				if ($user->type == 'student') {
					header(''); // Reffer to student index.
				}
				else if ($user->type == 'teacher') {
					header(''); // Reffer to teacher index.
				}
			}
			else {
				$this->_view->error = 'No such username || password.';
			}
		}
	
		$form = new Form();
		$form
			->label('ID: ')
			->validator('int')
			->attr('name', 'user_id')
			->element('text');
			
		$form
			->label('Password:')
			->attr('name', 'password')
			->element('password');
			
		$form
			->attr('value', 'Login')
			->element('submit');
		
		$this->_view->login_form = $form->setAsStaticForm();
	}

}

?>