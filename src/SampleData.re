open Belt;

let a =
  [
    ("Mary", "Joseph", 40.),
    ("Mary", "Michael", 40.),
    ("Joseph", "Michael", 60.),
    ("Joseph", "Gabriel", 55.),
    ("Michael", "Raphael", 55.),
    ("Gabriel", "Raphael", 50.),
    ("Mary", "Paul", 15.),
    ("Raphael", "Peter", 30.),
    ("Peter", "John", 10.),
    ("Paul", "James", 10.),
    ("Gabriel", "Andrew", 30.),
  ]
  ->List.keepMap(((i, j, w)) =>
      switch (Graph.Vertex.(make(i), make(j))) {
      | (Some(i), Some(j)) => Some((i, j, w))
      | _ => None
      }
    )
  ->Graph.fromList;
