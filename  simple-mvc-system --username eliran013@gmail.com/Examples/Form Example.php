<?php 
	/**
	 * Create a new form
	 */
	$form = new Form($action, $method, $id, $class);
	
	/**
	 * Make this: <input type="text" value="foo" id="bar" />
	 */
	$form->attr('value', 'foo')->attr('id', 'bar')->element('text');
	
	/**
	 * Make a <select> list, with a 1px solid black border.
	 */
	$form->attr('style', 'border: 1px solid black;')->option('Blue')->option('Green')->option('Red')->element('select');
	
	/**
	 * Using the label() method.
	 * I'll create a label that says "Username", with blue font.
	 */
	$form->attr('name', 'username')->label('Username', '<span style="color: blue">%s:</span> ')->element('text');
	
	/**
	 * Using the blocks
	 */	
	
	// Create the first block, and fill it with elements (<select> elements)
	// Notice that the first parameter ('block1'), is the block name, and the rest are elements.
	$form->set_block('block1',
		$form
		->attr('id', 'bla')
		->attr('style', 'border: 1px solid black;')
		->option('bla')
		->option('bla2')
		->element('select')->get(),
		$form
		->attr('id', 'bla')
		->attr('style', 'border: 1px solid black;')
		->option('bla')
		->option('bla2')
		->element('select')->get(),
		$form
		->attr('id', 'bla')
		->attr('style', 'border: 1px solid black;')
		->option('bla')
		->option('bla2')
		->element('select')->get()
	);
	
	// Create the second block, I'm lazy so I'll just fill it with the same elements.
	// Notice that the first parameter ('block2'), is the block name, and the rest are elements.
	$form->set_block('block2',
		$form
		->attr('id', 'bla')
		->attr('style', 'border: 1px solid black;')
		->option('bla')
		->option('bla2')
		->element('select')->get(),
		$form
		->attr('id', 'bla')
		->attr('style', 'border: 1px solid black;')
		->option('bla')
		->option('bla2')
		->element('select')->get(),
		$form
		->attr('id', 'bla')
		->attr('style', 'border: 1px solid black;')
		->label('bla', '<span style="color: blue;">%s:</span> ')
		->option('bla')
		->option('bla2')
		->element('select')->get()
	);
	
	// Now i'll define the format (each block is in a different div, with border and margin), and i'll print the block set.
	// Notice that the first parameter is the format, (%s stands for a block), and the rest are block names.
	echo $form->setAsBlockSetForm('<div style="border: 1px solid black; margin-bottom: 30px;">%s</div><div style="border: 1px solid black; margin-bottom: 30px;">%s</div>', 'block1', 'block2');
	
	
	/**
	 * Ways to print forms
	 */
	// This will return a blockset, after setting the blocks with the set_block() method.
	$form->setAsBlockSetForm($format, $block_name1, $block_name2, $block_name3); // and so on...
	
	// This will return all the elements between <form action..> and </form> tags.
	$form->setAsStaticForm();
	
	// * Not yet ready
	// This will return an ajax form.
	$form->setAsAjaxForm();
?>