CREATE TABLE IF NOT EXISTS users (
    uid INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    username VARCHAR(30) UNIQUE,
    password CHAR(60) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS drives (
    drivename NOT NULL PRIMARY KEY,
    user INTEGER,
    password CHAR(60),

    FOREIGN KEY (user) 
        REFERENCES users (uid) 
            ON DELETE CASCADE

);
