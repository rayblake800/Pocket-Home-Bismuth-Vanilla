# Application Menu Configuration
The menuOptions.json configuration file holds options that control the format and behavior of the application launch menu.

Key                          | Permitted Values      | Description
---------------------------- | --------------------- | ---
"AppMenu format"             | "Paged", "Scrolling"  | The application launch menu format used.
"scrolling AppMenu max rows" | Any positive integer. | Maximum number of menu rows to fit within the window when using the scrolling menu format.
"paged AppMenu max columns"  | Any positive integer. | Maximum number of menu columns to fit within a single page of the paged menu format.
"paged AppMenu max rows"     | Any positive integer. | Maximum number of menu rows to fit within a single page of the paged menu format.
"application launch timeout" | Any positive integer. | Milliseconds to wait before assuming an application launch failed.
