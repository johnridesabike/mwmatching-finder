open Belt;
[@bs.module] external nanoid: unit => string = "nanoid";

module Edge = {
  type t = (string, string);
  module Cmp =
    Id.MakeComparable({
      type nonrec t = t;
      let cmp: (t, t) => int =
        ((a, b), (y, z)) =>
          switch (compare(a, y), compare(b, z)) {
          | (0, 0) => 0
          | _ =>
            switch (compare(a, z), compare(b, y)) {
            | (0, 0) => 0
            | (c, d) =>
              switch (c + d) {
              | 0 => c
              | e => e
              }
            }
          };
    });
};

module Graph = {
  module S = Set.String;
  module M = Map;

  type t = {
    vertices: S.t,
    edges: M.t(Edge.t, float, Edge.Cmp.identity),
  };

  let empty = {vertices: S.empty, edges: M.make(~id=(module Edge.Cmp))};

  type action =
    | AddVertex(string)
    | AddEdge(string, string, float)
    | RemoveVertex(string)
    | RemoveEdge(string, string);

  let reducer = ({vertices, edges} as graph, action) =>
    switch (action) {
    | AddVertex(vertex) => {...graph, vertices: S.add(vertices, vertex)}
    | AddEdge(i, j, weight) => {
        vertices: vertices->S.add(i)->S.add(j),
        edges: M.set(edges, (i, j), weight),
      }
    | RemoveVertex(vertex) => {
        vertices: S.remove(vertices, vertex),
        edges:
          M.keep(edges, ((i, j), _weight) => i !== vertex && j !== vertex),
      }
    | RemoveEdge(i, j) => {...graph, edges: M.remove(edges, (i, j))}
    };

  let fromList = l => {
    let rec loop = (acc, l) =>
      switch (l) {
      | [] => acc
      | [(i, j, w), ...l] => loop(reducer(acc, AddEdge(i, j, w)), l)
      };
    loop(empty, l);
  };

  let toList = ({edges, _}) =>
    M.reduce(edges, [], (acc, (i, j), w) => [(i, j, w), ...acc]);

  let verticesToList = ({vertices, _}) => S.toList(vertices);
};

let sampleGraph =
  Graph.fromList([
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
  ]);

module NameForm = {
  open Formality;
  type field =
    | Name;
  type state = {name: string};
  type message = string;
  type submissionError = unit;

  module NameField = {
    let update = (_state, value) => {name: value};

    let validator = {
      field: Name,
      strategy: Strategy.OnFirstBlur,
      dependents: None,
      validate: ({name}) =>
        switch (name) {
        | "" => Error("Error")
        | _ => Ok(Valid)
        },
    };
  };

  let validators = [NameField.validator];
};

module NameFormHook = Formality.Make(NameForm);

module MatchForm = {
  open Formality;
  type field =
    | I
    | J
    | W;
  type state = {
    i: string,
    j: string,
    w: option(float),
  };
  type message = string;
  type submissionError = unit;

  module IField = {
    let update = (state, value) => {...state, i: value};

    let validator = {
      field: I,
      strategy: Strategy.OnFirstChange,
      dependents: None,
      validate: ({i, j, _}) =>
        if (i == j) {
          Error("They can't be the same");
        } else {
          Ok(Valid);
        },
    };
  };

  module JField = {
    let update = (state, value) => {...state, j: value};

    let validator = {
      field: J,
      strategy: Strategy.OnFirstChange,
      dependents: None,
      validate: ({i, j, _}) =>
        if (i == j) {
          Error("They can't be the same");
        } else {
          Ok(Valid);
        },
    };
  };
  module WField = {
    let update = (state, value) => {...state, w: value};

    let validator = {
      field: W,
      strategy: Strategy.OnFirstChange,
      dependents: None,
      validate: ({w, _}) =>
        switch (w) {
        | None => Error("Weight must be a valid number")
        | Some(_) => Ok(Valid)
        },
    };
  };

  let validators = [IField.validator, JField.validator, WField.validator];
};

module MatchFormHook = Formality.Make(MatchForm);

module MatchAdder = {
  [@react.component]
  let make = (~graph, ~dispatch) => {
    open MatchFormHook;
    open MatchForm;
    let form =
      useForm(
        ~initialState={i: "", j: "", w: Some(0.0)},
        ~onSubmit=({i, j, w}, form) => {
          open! Formality__Validation;
          switch (w) {
          | Some(w) => dispatch(Graph.AddEdge(i, j, w))
          | None => ()
          };
          form.notifyOnSuccess(None);
          form.reset();
        },
      );
    let nameList =
      graph
      ->Graph.verticesToList
      ->List.map(p => <option key=p value=p> p->React.string </option>)
      ->List.toArray
      ->React.array;
    <form onSubmit={form.submit->Formality.Dom.preventDefault}>
      <select
        value={form.state.i}
        onChange={event =>
          form.change(
            I,
            IField.update(form.state, event->ReactEvent.Form.target##value),
          )
        }>
        nameList
      </select>
      " + "->React.string
      <select
        value={form.state.j}
        onChange={event =>
          form.change(
            J,
            JField.update(form.state, event->ReactEvent.Form.target##value),
          )
        }>
        nameList
      </select>
      <input
        type_="number"
        value={form.state.w->Option.getWithDefault(0.)->Js.String.make}
        onChange={event =>
          form.change(
            W,
            WField.update(
              form.state,
              event->ReactEvent.Form.target##value->Float.fromString,
            ),
          )
        }
      />
      <button> "Submit"->React.string </button>
      {switch (form.result(I), form.result(J), form.result(W)) {
       | (Some(Error(message)), _, _)
       | (_, Some(Error(message)), _)
       | (_, _, Some(Error(message))) => <p> message->React.string </p>
       | (Some(Ok(_)), _, _)
       | (_, Some(Ok(_)), _)
       | (_, _, Some(Ok(_)))
       | (None, None, None) => React.null
       }}
    </form>;
  };
};

[@react.component]
let make = () => {
  let (graph, dispatch) = React.useReducer(Graph.reducer, sampleGraph);
  let (matches, setMatches) =
    React.useState(() => graph->Graph.toList->Blossom.Match.String.make);
  React.useEffect1(
    () => {
      setMatches(state =>
        switch (graph->Graph.toList->Blossom.Match.String.make) {
        | exception e =>
          Js.Console.error(e);
          state;
        | matches => matches
        }
      );
      None;
    },
    [|graph|],
  );
  open NameFormHook;
  let form =
    useForm(
      ~initialState=NameForm.{name: ""},
      ~onSubmit=(NameForm.{name}, form) => {
        open! Formality__Validation;
        dispatch(Graph.AddVertex(name));
        form.notifyOnSuccess(None);
        form.reset();
      },
    );
  <div>
    <form onSubmit={form.submit->Formality.Dom.preventDefault}>
      <input
        value={form.state.NameForm.name}
        disabled={form.submitting}
        onBlur={_ => form.blur(NameForm.Name)}
        onChange={event =>
          form.change(
            NameForm.Name,
            NameForm.NameField.update(
              form.state,
              event->ReactEvent.Form.target##value,
            ),
          )
        }
      />
      <button disabled={form.submitting}>
        (form.submitting ? "Submitting..." : "Submit")->React.string
      </button>
    </form>
    {switch (form.result(NameForm.Name)) {
     | Some(Error(message)) => message->React.string
     | Some(Ok(Formality.(Valid | NoValue)))
     | None => React.null
     }}
    <h2> "People"->React.string </h2>
    <ul>
      {graph
       ->Graph.verticesToList
       ->List.map(p => <li key=p> p->React.string </li>)
       ->List.toArray
       ->React.array}
    </ul>
    <h2> "Add match"->React.string </h2>
    <MatchAdder graph dispatch />
    <h2> "Matches"->React.string </h2>
    <ul>
      {matches
       ->Blossom.Match.toList
       ->List.mapWithIndex((key, (a, b)) =>
           <li key={Js.String.make(key)}>
             {(a ++ " + " ++ b)->React.string}
           </li>
         )
       ->List.toArray
       ->React.array}
    </ul>
  </div>;
};
