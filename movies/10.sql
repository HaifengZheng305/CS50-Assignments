SELECT name
FROM people
JOIN directors on people.id = directors.person_id
JOIN ratings on ratings.movie_id = directors.movie_id
WHERE rating >= 9;