Select TITLE
FROM movies
JOIN stars on stars.movie_id = movies.id
JOIN  people on stars.person_id = people.id
WHERE people.name = 'Johnny Depp'
INTERSECT
Select TITLE
FROM movies
JOIN stars on stars.movie_id = movies.id
JOIN  people on stars.person_id = people.id
WHERE people.name = 'Helena Bonham Carter';