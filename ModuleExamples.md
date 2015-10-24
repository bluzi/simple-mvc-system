# Module Examples #
**Please read [FirstSteps](FirstSteps.md) First.**

## What is a Module? ##
A module, or a model, is a class which implements an object in the system. To list a few examples: User, Blog Post, Product, Category, Page, or even DOM elements, like a table.

For a more comprehensive example, we will refer to the blog post mentioned above: A blog post might have a title, it has textual content, an author and possibly a posting date. For indexing purposes, it also has a unique I.D.
There are several operations which could be related to the said blog post: it could be created, edited, deleted and so on.

For the sake of the example, we will assume that there are blog posts filled in our database, and that there is a method of BlogPost which edits the title & content of the post, called "edit".

We will implement a function which retrieves one of them (by it's I.D) from the database, and performs operations on it:

```
<?php 

	public function blogpostAction() {
		$blogpost = new BlogPost($_GET['id']);
		echo $blogpost->title;
		$blogpost->edit($blogpost->title . ' (!)', $blogpost->content);
		if ($_GET['delete'] === 1) {
			$blogpost->delete();
		}
	}

?>
```

## Empty Module Example ##
```
<?php 
/**
 * @name user.module.inc
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

	// Variables

	/**
	 * The user type
	 */
	public $user_id= '';

	// Methods

	public function __construct($user_id) {
		// Code here
	}

}
?>
```

## Module Format ##
All the modules are placed at: "/Appliaction/Modules/", the naming format is: "user.module.inc" (Replace the "user" with the module name).
Also, the class name should be "userModule", where "user" is the module name.

Please try to only use lower case letters in the file naming.

## Calling a Module ##
Generally, Modules should only be called by Actions, located in the Controllers (see [FirstSteps](FirstSteps.md)), however it is possible to call them from anywhere at the code (Even from the Views section).

To call a Module, simply implement a new instance (using PHP "new" reserved word). The class name should be "userModule". (when "user" is the module name)

```
<?php
	$user = new userModule(42);
?>
```