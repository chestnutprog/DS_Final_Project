# HTTP Based Server

Server use a random token to indentifier different client.

Logged in users' token should restore in the database.

Player could play in anonymous. While their nickname should be anonymous.

POST `/login/`

GET `/ranklist/:page`

GET `/userinfo[/:id]`

WS `/main_game`