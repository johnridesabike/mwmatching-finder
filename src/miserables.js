module.exports = {
  "nodes": [
    "Andrew",
    "Gabriel",
    "James",
    "John",
    "Joseph",
    "Mary",
    "Michael",
    "Paul",
    "Peter",
    "Raphael"
  ],
  "links": [
    {"source": "Mary", "target": "Joseph", "weight": 40},
    {"source": "Mary", "target": "Michael", "weight": 40},
    {"source": "Joseph", "target": "Michael", "weight": 60},
    {"source": "Joseph", "target": "Gabriel", "weight": 55},
    {"source": "Michael", "target": "Raphael", "weight": 55},
    {"source": "Gabriel", "target": "Raphael", "weight": 50},
    {"source": "Mary", "target": "Paul", "weight": 15},
    {"source": "Raphael", "target": "Peter", "weight": 30},
    {"source": "Peter", "target": "John", "weight": 10},
    {"source": "Paul", "target": "James", "weight": 10},
    {"source": "Gabriel", "target": "Andrew", "weight": 30}
  ]
}

