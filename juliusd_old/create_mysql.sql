-- drop database juliusd;
create database juliusd;
use juliusd;

create table User (
	UserId int primary key AUTO_INCREMENT,
	Username varchar(50) not null,
	Password varchar(250) not null
);
