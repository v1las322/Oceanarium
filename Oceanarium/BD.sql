create database ocean;
use ocean;
-- Создание таблицы для пользователей
CREATE TABLE Users (
    UserID INT AUTO_INCREMENT PRIMARY KEY,
    Username VARCHAR(255) NOT NULL UNIQUE,
    Password VARCHAR(255) NOT NULL,
    Email VARCHAR(255) NOT NULL UNIQUE
);

-- Создание таблицы для обитателей
CREATE TABLE Residents (
  ResidentID INT AUTO_INCREMENT PRIMARY KEY,
  ImageURL VARCHAR(255),
  Name VARCHAR(255) NOT NULL,
  Species VARCHAR(255) NOT NULL,
  Age INT,
  Description TEXT
);
