Select DISTINCT(name)
FROM people
JOIN stars on stars.person_id = people.id
JOIN movies on movies.id = stars.movie_id
WHERE movies.id IN(
SELECT movies.id
FROM movies
JOIN stars on stars.movie_id = movies.id
JOIN  people on stars.person_id = people.id
WHERE name like 'Kevin Bacon' and birth = 1958)
AND name !='Kevin Bacon' ;