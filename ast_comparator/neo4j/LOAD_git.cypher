
// NOT WORKING ON A GITHUB
LOAD CSV WITH HEADERS FROM "https://raw.githubusercontent.com/alapha23/tiny-comparator/master/ast_comparator/neo4j/sample.csv" AS row
CREATE (n:Students)
SET n = row,
 n.similarity = toFloat(row.similarity)
 n.number = toInteger(row.number)


// DELETE ALL NODES
MATCH (n) DETACH DELETE n

// DELETE ALL NODES EXCEPT nodes with tag of Students
MATCH (n) WHERE (n:Students) DETACH DELETE n


