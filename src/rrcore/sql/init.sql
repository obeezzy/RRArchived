USE mysql;

-- Create database
DROP DATABASE IF EXISTS rr;
CREATE DATABASE rr;

USE rr;

START TRANSACTION;
-- Create business details table
DROP TABLE IF EXISTS business_details;
CREATE TABLE business_details (
    id INT(11) NOT NULL AUTO_INCREMENT,
    address VARCHAR(200) NOT NULL,
    type VARCHAR(500) DEFAULT NULL,
    establishment_year INT(11) NOT NULL,
    phone_number VARCHAR(20) DEFAULT NULL,
    logo BLOB,
    extra_details VARCHAR(100) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create category table
DROP TABLE IF EXISTS category;
CREATE TABLE category (
    id INT(11) NOT NULL AUTO_INCREMENT,
    category VARCHAR(100) NOT NULL,
    short_form VARCHAR(25) DEFAULT NULL,
    note_id INT(11) DEFAULT NULL,
    archived INT(11) NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE KEY category (category)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create client table
DROP TABLE IF EXISTS client;
CREATE TABLE client (
    id INT(11) NOT NULL AUTO_INCREMENT,
    first_name VARCHAR(100) DEFAULT NULL,
    last_name VARCHAR(100) DEFAULT NULL,
    preferred_name VARCHAR(100) NOT NULL,
    phone_number VARCHAR(20) NOT NULL,
    address VARCHAR(100) DEFAULT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL DEFAULT 0,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE KEY phone_number (phone_number)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create credit payment table
DROP TABLE IF EXISTS credit_payment;
CREATE TABLE credit_payment (
    id INT(11) NOT NULL AUTO_INCREMENT,
    credit_transaction_id INT(11) NOT NULL,
    total_debt DECIMAL(19,2) NOT NULL,
    amount_paid DECIMAL(19,2) NOT NULL,
    balance DECIMAL(19,2) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    due_date DATETIME NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create creditor table
DROP TABLE IF EXISTS creditor;
CREATE TABLE creditor (
    id INT(11) NOT NULL AUTO_INCREMENT,
    client_id INT(11) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create credit transaction table
DROP TABLE IF EXISTS credit_transaction;
CREATE TABLE credit_transaction (
    id INT(11) NOT NULL AUTO_INCREMENT,
    creditor_id INT(11) NOT NULL,
    transaction_table VARCHAR(20) NOT NULL,
    transaction_id INT(11) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create current quantity table
DROP TABLE IF EXISTS current_quantity;
CREATE TABLE current_quantity (
    id INT(11) NOT NULL AUTO_INCREMENT,
    item_id INT(11) DEFAULT NULL,
    quantity DOUBLE NOT NULL,
    unit_id INT(11) NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE KEY item_id (item_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create customer table
DROP TABLE IF EXISTS customer;
CREATE TABLE customer (
    id INT(11) NOT NULL AUTO_INCREMENT,
    client_id INT(11) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE KEY client_id (client_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create damaged quantity table
DROP TABLE IF EXISTS damaged_quantity;
CREATE TABLE damaged_quantity (
    id INT(11) NOT NULL AUTO_INCREMENT,
    item_id INT(11) DEFAULT NULL,
    quantity DOUBLE NOT NULL,
    unit_id INT(11) NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE KEY item_id (item_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create DB info table
DROP TABLE IF EXISTS db_info;
CREATE TABLE db_info (
    version VARCHAR(20) NOT NULL,
    rack_id VARCHAR(40) NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    PRIMARY KEY (version)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create debt payment table
DROP TABLE IF EXISTS debt_payment;
CREATE TABLE debt_payment (
    id INT(11) NOT NULL AUTO_INCREMENT,
    debt_transaction_id INT(11) NOT NULL,
    total_amount DECIMAL(19,2) NOT NULL,
    amount_paid DECIMAL(19,2) NOT NULL,
    balance DECIMAL(19,2) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    due_date DATETIME NOT NULL,
    note_id INT(11) NOT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create debt transaction table
DROP TABLE IF EXISTS debt_transaction;
CREATE TABLE debt_transaction (
    id INT(11) NOT NULL AUTO_INCREMENT,
    debtor_id INT(11) NOT NULL,
    transaction_table VARCHAR(20) NOT NULL,
    transaction_id INT(11) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create debtor table
DROP TABLE IF EXISTS debtor;
CREATE TABLE debtor (
    id INT(11) NOT NULL AUTO_INCREMENT,
    client_id INT(11) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create expense table
DROP TABLE IF EXISTS expense;
CREATE TABLE expense (
    id INT(11) NOT NULL AUTO_INCREMENT,
    name VARCHAR(30) DEFAULT NULL,
    client_id INT(11) DEFAULT 0,
    purpose VARCHAR(100) NOT NULL,
    total_amount DECIMAL(19,2) NOT NULL,
    amount_paid DECIMAL(19,2) NOT NULL,
    balance DECIMAL(19,2) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL DEFAULT 0,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create expense payment table
DROP TABLE IF EXISTS expense_payment;
CREATE TABLE expense_payment (
    id INT(11) NOT NULL AUTO_INCREMENT,
    expense_id INT(11) NOT NULL,
    amount INT(11) NOT NULL,
    method VARCHAR(20) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create expense purpose table
DROP TABLE IF EXISTS expense_purpose;
CREATE TABLE expense_purpose (
    id INT(11) NOT NULL AUTO_INCREMENT,
    purpose VARCHAR(100) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create income table
DROP TABLE IF EXISTS income;
CREATE TABLE income (
    id INT(11) NOT NULL AUTO_INCREMENT,
    name VARCHAR(30) DEFAULT NULL,
    client_id INT(11) DEFAULT 0,
    purpose VARCHAR(100) NOT NULL,
    total_amount DECIMAL(19,2) NOT NULL,
    amount_paid DECIMAL(19,2) NOT NULL,
    balance DECIMAL(19,2) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL DEFAULT 0,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create income payment table
DROP TABLE IF EXISTS income_payment;
CREATE TABLE income_payment (
    id INT(11) NOT NULL AUTO_INCREMENT,
    income_id INT(11) NOT NULL,
    amount INT(11) NOT NULL,
    method VARCHAR(20) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create income purpose table
DROP TABLE IF EXISTS income_purpose;
CREATE TABLE income_purpose (
    id INT(11) NOT NULL AUTO_INCREMENT,
    purpose VARCHAR(100) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create initial quantity table
DROP TABLE IF EXISTS initial_quantity;
CREATE TABLE initial_quantity (
    id INT(11) NOT NULL AUTO_INCREMENT,
    item_id INT(11) NOT NULL,
    quantity DOUBLE NOT NULL,
    unit_id INT(11) NOT NULL,
    reason VARCHAR(30) NOT NULL,
    archived TINYINT NOT NULL DEFAULT 0,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create item table
DROP TABLE IF EXISTS item;
CREATE TABLE item (
    id INT(11) NOT NULL AUTO_INCREMENT,
    category_id INT(11) NOT NULL,
    item VARCHAR(200) NOT NULL,
    short_form VARCHAR(10) DEFAULT NULL,
    description VARCHAR(200) DEFAULT NULL,
    barcode VARCHAR(70) DEFAULT NULL,
    divisible TINYINT DEFAULT 1,
    image BLOB,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE KEY item (item),
    UNIQUE KEY barcode (barcode)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create last used date/time table
DROP TABLE IF EXISTS last_used_date_time;
CREATE TABLE last_used_date_time (
    last_date_time DATETIME NOT NULL,
    created DATETIME NOT NULL,
    PRIMARY KEY (last_date_time)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create note table
DROP TABLE IF EXISTS note;
CREATE TABLE note (
    id INT(11) NOT NULL AUTO_INCREMENT,
    note VARCHAR(200) NOT NULL,
    table_name VARCHAR(30) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create purchase item table
DROP TABLE IF EXISTS purchase_item;
CREATE TABLE purchase_item (
    id INT(11) NOT NULL AUTO_INCREMENT,
    purchase_transaction_id INT(11) NOT NULL,
    item_id INT(11) NOT NULL,
    unit_price DECIMAL(19,2) NOT NULL,
    quantity DOUBLE NOT NULL,
    cost DOUBLE NOT NULL,
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL DEFAULT 0,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create purchase payment table
DROP TABLE IF EXISTS purchase_payment;
CREATE TABLE purchase_payment (
    id INT(11) NOT NULL AUTO_INCREMENT,
    purchase_transaction_id INT(11) NOT NULL,
    amount INT(11) NOT NULL,
    method VARCHAR(20) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited INT(11) NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create purchase transaction table
DROP TABLE IF EXISTS purchase_transaction;
CREATE TABLE purchase_transaction (
    id INT(11) NOT NULL AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    client_id INT(11) DEFAULT NULL,
    total_cost DECIMAL(19,2) NOT NULL,
    amount_paid DECIMAL(19,2) NOT NULL,
    balance DECIMAL(19,2) NOT NULL,
    discount DECIMAL(19,2) NOT NULL,
    suspended TINYINT NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create sale item table
DROP TABLE IF EXISTS sale_item;
CREATE TABLE sale_item (
    id INT(11) NOT NULL AUTO_INCREMENT,
    sale_transaction_id INT(11) NOT NULL,
    item_id INT(11) NOT NULL,
    unit_price DECIMAL(19,2) NOT NULL,
    quantity DOUBLE NOT NULL,
    unit_id INT(11) NOT NULL,
    cost DECIMAL(19,2) NOT NULL,
    discount DECIMAL(19,2) NOT NULL DEFAULT '0.00',
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create sale payment table
DROP TABLE IF EXISTS sale_payment;
CREATE TABLE sale_payment (
    id INT(11) NOT NULL AUTO_INCREMENT,
    sale_transaction_id INT(11) NOT NULL,
    amount DECIMAL(19,2) NOT NULL,
    method VARCHAR(20) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create sale transaction table
DROP TABLE IF EXISTS sale_transaction;
CREATE TABLE sale_transaction (
    id INT(11) NOT NULL AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    client_id INT(11) DEFAULT NULL,
    total_cost DECIMAL(19,2) NOT NULL,
    amount_paid DECIMAL(19,2) NOT NULL,
    balance DECIMAL(19,2) NOT NULL,
    change_due DECIMAL(19,2) DEFAULT '0.00',
    discount DECIMAL(19,2) NOT NULL,
    suspended TINYINT NOT NULL,
    note_id INT(11) NOT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create unit tale
DROP TABLE IF EXISTS unit;
CREATE TABLE unit (
    id INT(11) NOT NULL AUTO_INCREMENT,
    item_id INT(11) NOT NULL,
    unit VARCHAR(30) NOT NULL,
    short_form VARCHAR(10) DEFAULT NULL,
    base_unit_equivalent INT(11) NOT NULL,
    preferred TINYINT NOT NULL DEFAULT 0,
    cost_price DECIMAL(19,2) NOT NULL,
    retail_price DECIMAL(19,2) NOT NULL,
    currency VARCHAR(4) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create unit relation table
DROP TABLE IF EXISTS unit_relation;
CREATE TABLE unit_relation (
    id INT(11) NOT NULL AUTO_INCREMENT,
    item_id INT(11) NOT NULL,
    old_unit_quantity DOUBLE NOT NULL,
    old_unit_id INT(11) NOT NULL,
    new_unit_quantity DOUBLE NOT NULL,
    new_unit_id INT(11) NOT NULL,
    note_id INT(11) NOT NULL DEFAULT 0,
    archived TINYINT NOT NULL DEFAULT 0,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create user table
DROP TABLE IF EXISTS user;
CREATE TABLE user (
    id INT(11) NOT NULL AUTO_INCREMENT,
    user VARCHAR(60) DEFAULT NULL,
    photo BLOB DEFAULT NULL,
    phone_number VARCHAR(20) DEFAULT NULL,
    email_address VARCHAR(30) DEFAULT NULL,
    active TINYINT DEFAULT 0,
    pending TINYINT DEFAULT 0,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create user privilege table
DROP TABLE IF EXISTS user_privilege;
CREATE TABLE user_privilege (
    id INT(11) NOT NULL AUTO_INCREMENT,
    user_id INT(11) NOT NULL,
    privileges VARCHAR(1000) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE KEY user_id (user_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Create vendor table
DROP TABLE IF EXISTS vendor;
CREATE TABLE vendor (
    id INT(11) NOT NULL AUTO_INCREMENT,
    client_id INT(11) NOT NULL,
    note_id INT(11) DEFAULT NULL,
    archived TINYINT NOT NULL,
    created DATETIME NOT NULL,
    last_edited DATETIME NOT NULL,
    user_id INT(11) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE client_id (client_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

COMMIT;