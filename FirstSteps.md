# First Steps #
**If you need any installation instructions, please refer to [Installation](Installation.md).**

## Structure ##
Each page is separated to at least 2 parts, which will be discussed in this guide.

- **Action** - The functional code, the "engine" of the page.

- **View** - The graphical part, or: What the user sees & interacts with.

The missing part is the module (or model) part, and you might want to read more about it in the [ModuleExamples](ModuleExamples.md) page.

## Controllers ##
The actions are ordered by the controllers, which are "page categories". For example, if we take a look at a web boards system (forums), the pages: "Login", "Profile" and "Logout" will be placed in the User Controller, and the pages: "Topicslist", "Topic" and "Reply", will be placed in the ForumController".

A basic controller should look something like this:

### /Application/Controllers/user.controller.inc ###
```
<?php 
class userController extends Controller {

	/**
	 * The login page
	 */
	public function loginAction() {
		// Code here
	}
	
	/**
	 * The profile page
	 */
	public function profileAction() {
		// Code here
	}
	
	/**
	 * The logout script
	 */
	public function logoutAction() {
		// Code here
	}
	

}

?>
```

**The controller and action names are important:
Controller name should be: "x.controller.inc", and action name should be: "xAction".**

**Also, the name of the controller class is important. It should be "xController". (x = the given name)**

**Please try to keep all the names in one format. I suggest lower case letters.**

**One more thing about controllers, is that they must be placed in /Application/Controllers/.**

## Views ##
After you wrote the action code, you will probably want to show the user the results.
So, views will be placed in /Application/Views/. The naming format is "action.controller.inc" (action = the action name, controller = the controller name).

To pass variables between actions and views, you use the variable $this->_view in the action part._

For example, if I want to pass information about the user in the profile action, I'll do something like that:
(Notice the profileAction)
```
<?php 
class userController extends Controller {

	/**
	 * The login page
	 */
	public function loginAction() {
		// Code here
	}
	
	/**
	 * The profile page
	 */
	public function profileAction() {
		$user_information = array(
			'username' => 'Mr.bluz',
			'first_name' => 'Eliran',
			'age' => '18',
		);
		
		$this->_view->user_information = $user_information;
	}
	
	/**
	 * The logout script
	 */
	public function logoutAction() {
		// Code here
	}
	

}

?>
```

So, after we passed the information from the action, we would like to call it from the view, and print it on the screen.
In order to do that, we will use the variable $view, in the view part.

Example:
### /Application/Views/profile.uesr.inc ###
```
<h1><?php $view->user_information['username']?>'s Profile</h1>
<strong>Name:</strong> <?php $view->user_information['first_name'];?>.<br />
<strong>Agee:</strong> <?php $view->user_information['age'];?>.
```

This will print:
<h1>Mr.bluz's Profile</h1>
<strong>Name:</strong> Eliran.<br />
<strong>Agee:</strong> 18.