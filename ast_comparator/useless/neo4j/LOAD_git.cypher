
// Naming conventions
// Node labels: :VehicleOwner rather than vehice_owner
// Relationship types: :OWNS_VEHICLE rather ownsVehicle

// LOAD csv file
LOAD CSV WITH HEADERS FROM "https://raw.githubusercontent.com/alapha23/tiny-comparator/master/ast_comparator/neo4j/sample.csv" AS row1
CREATE (n:Plagiarism)
SET n = row1,
 n.similarity = toFloat(row1.similarity),
 n.number = toInteger(row1.number)

LOAD CSV WITH HEADERS FROM "https://raw.githubusercontent.com/alapha23/tiny-comparator/master/ast_comparator/neo4j/student_list.csv" AS row2
CREATE (k:Students)
SET k = row2

// LINK csv files together
CREATE INDEX ON :Students(id)
// ANY nodes with "id" property would be based on this index

// CREATE edges
MATCH (p:Plagiarism), (s:Students)
WHERE p.student = s.student AND p.judgement = "Suspicious"
CREATE (s)-[:SUSPICIOUS]->(p)

MATCH (p:Plagiarism), (s:Students)
WHERE p.student = s.student AND p.judgement = "Plagiarism"
CREATE (s)-[:PLAGIARISM]->(p)




// 

// DELETE ALL NODES
MATCH (n) DETACH DELETE n

// DELETE ALL NODES EXCEPT nodes with tag of Students
MATCH (n) WHERE (n:Students) DETACH DELETE n


