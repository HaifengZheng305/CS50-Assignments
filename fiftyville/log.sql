-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT DISTINCT(people.id), name, people.license_plate
FROM people
JOIN bakery_security_logs on people.license_plate = bakery_security_logs.license_plate
WHERE year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 15 and minute <= 30;


select * from crime_scene_reports
WHERE year = 2021 and month = 7 and day = 28;

select * from interviews
WHERE year = 2021 and month = 7 and day = 28;

SELECT *
FROM people
JOIN bakery_security_logs on people.license_plate = bakery_security_logs.license_plate
WHERE year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 15 and minute <= 25 and phone_number in(
    Select caller from phone_calls where duration <= 60 and year = 2021 and month = 7 and day = 28
);

Select * from phone_calls where duration <= 60 and year = 2021 and month = 7 and day = 28

SELECT account_number, person_id
FROM bank_accounts
WHERE person_id in (
SELECT people.id
FROM people
JOIN bakery_security_logs on people.license_plate = bakery_security_logs.license_plate
WHERE year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 15 and minute <= 25 and phone_number in(
    Select caller from phone_calls where duration <= 60 and year = 2021 and month = 7 and day = 28
));


SELECT flight_id, passport_number
FROM passengers
Where passport_number in(
SELECT passport_number
FROM people
WHERE id in(SELECT person_id
FROM bank_accounts
WHERE person_id in (
SELECT people.id
FROM people
JOIN bakery_security_logs on people.license_plate = bakery_security_logs.license_plate
WHERE year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 15 and minute <= 25 and phone_number in(
    Select caller from phone_calls where duration <= 60 and year = 2021 and month = 7 and day = 28
))));

Select *
FROM flights
WHERE id = 54;

SELECT *
FROM airports
WHERE ID = 4
;

Select * from people where phone_number like '(375) 555-8161';


