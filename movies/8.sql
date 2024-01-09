SELECT name
FROM people
join stars on people.id = stars.person_id
where movie_id = (SELECT id from movies where title like 'toy story');
