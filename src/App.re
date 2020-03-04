open Belt;

let focusRef = ref =>
  Webapi.Dom.(
    ref
    ->React.Ref.current
    ->Js.Nullable.toOption
    ->Option.flatMap(Element.asHtmlElement)
    ->Option.map(HtmlElement.focus)
    ->ignore
  );

module Graph = {
  module M = Map;
  module S = Set;

  module Edge = {
    type t = (string, string);
    module Cmp =
      Id.MakeComparable({
        type nonrec t = t;
        let cmp: (t, t) => int =
          ((a, b), (y, z)) =>
            switch (compare(a, y), compare(b, z)) {
            | (0, 0) => 0
            | (c, d) =>
              switch (c + d) {
              | 0 => c
              | e => e
              }
            };
      });
    let make = (i, j) =>
      switch (String.compare(i, j)) {
      | 1 => (i, j)
      | _ => (j, i)
      };
  };

  type t = {
    /* A map of vertices to their neighbors */
    vertices: S.String.t,
    edges: M.t(Edge.t, float, Edge.Cmp.identity),
  };

  let empty = {
    vertices: S.String.empty,
    edges: M.make(~id=(module Edge.Cmp)),
  };

  type action =
    | AddVertex(string)
    | SetEdge(string, string, float)
    | RemoveEdge(string, string)
    | RemoveVertex(string);

  let reducer = ({vertices, edges} as graph, action) =>
    switch (action) {
    | AddVertex(vertex) => {
        ...graph,
        vertices: S.String.add(vertices, vertex),
      }
    | SetEdge(i, j, weight) => {
        vertices: vertices->S.String.add(i)->S.String.add(j),
        edges: M.set(edges, Edge.make(i, j), weight),
      }
    | RemoveEdge(i, j) => {
        vertices,
        edges: M.remove(edges, Edge.make(i, j)),
      }
    | RemoveVertex(vertex) => {
        ...graph,
        vertices: S.String.remove(vertices, vertex),
      }
    };

  let fromList = l => {
    let rec loop = (acc, l) =>
      switch (l) {
      | [] => acc
      | [(i, j, w), ...l] => loop(reducer(acc, SetEdge(i, j, w)), l)
      };
    loop(empty, l);
  };

  let toList = ({edges, _}) =>
    M.reduce(edges, [], (acc, (i, j), w) => [(i, j, w), ...acc]);

  let verticesToArray = ({vertices, _}) => S.String.toArray(vertices);
  let getEdge = ({edges, _}, i, j) => M.get(edges, Edge.make(i, j));
};

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
    /* let update = (state, value) => {...state, i: value};*/

    let validator = {
      field: I,
      strategy: Strategy.OnFirstChange,
      dependents: None,
      validate: ({i, _}) =>
        switch (i) {
        | "" => Error("Pick a name")
        | _ => Ok(Valid)
        },
    };
  };

  module JField = {
    /* let update = (state, value) => {...state, j: value};*/

    let validator = {
      field: J,
      strategy: Strategy.OnFirstChange,
      dependents: None,
      validate: ({i, j, _}) =>
        switch (i, j) {
        | (_, "") => Error("Pick a name")
        | (i, j) when i == j => Error("Names must be different")
        | (_, _) => Ok(Valid)
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
  let make = (~dispatch, ~onBlur, ~i, ~j, ~w, ~focused) => {
    open MatchFormHook;
    open MatchForm;
    let form =
      useForm(
        ~initialState={i, j, w},
        ~onSubmit=({i, j, w}, form) => {
          open! Formality__Validation;
          switch (w) {
          | Some(w) => dispatch(Graph.SetEdge(i, j, w))
          | None => ()
          };
          form.notifyOnSuccess(None);
          form.reset();
        },
      );
    <form onSubmit={form.submit->Formality.Dom.preventDefault}>
      <input type_="hidden" name="i" value=i />
      <input type_="hidden" name="j" value=j />
      <input
        className="match-adder__weight"
        type_="number"
        value={form.state.w->Option.getWithDefault(0.)->Js.String.make}
        ref={ReactDOMRe.Ref.domRef(focused)}
        onBlur
        onChange={event => {
          form.change(
            W,
            WField.update(
              form.state,
              event->ReactEvent.Form.target##value->Float.fromString,
            ),
          );
          form.submit();
        }}
      />
    </form>;
  };
};

module PersonAdder = {
  [@react.component]
  let make = (~dispatch) => {
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
    <form onSubmit={form.submit->Formality.Dom.preventDefault}>
      <p> "Add a person"->React.string </p>
      <input
        value={form.state.NameForm.name}
        disabled={form.submitting}
        onBlur={_ => form.blur(NameForm.Name)}
        size=10
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
      <p>
        {switch (form.result(NameForm.Name)) {
         | Some(Error(message)) => message->React.string
         | Some(Ok(Formality.(Valid | NoValue)))
         | None => React.null
         }}
      </p>
    </form>;
  };
};

module Table = {
  [@react.component]
  let make = (~vertices, ~cell) => {
    let verticesIndices = Array.size(vertices) - 1;
    /*         👆 now there's a fun name. */
    <table className="graph-table">
      <tbody>
        <tr>
          <th className="graph-table__corner"> "Name"->React.string </th>
          {vertices
           ->Array.reverse
           ->Array.slice(~len=verticesIndices, ~offset=0)
           ->Array.map(p =>
               <th key=p className="graph-table__top-name">
                 p->React.string
               </th>
             )
           ->React.array}
        </tr>
        {vertices
         ->Array.slice(~len=verticesIndices, ~offset=0)
         ->Array.mapWithIndex((i, p) =>
             <tr key=p>
               <th className="graph-table__side-name"> p->React.string </th>
               {vertices
                ->Array.reverse
                ->Array.slice(~len=verticesIndices, ~offset=0)
                ->Array.mapWithIndex((i', p') =>
                    if (verticesIndices - i' <= i) {
                      <td key=p' className="graph-table__null-cell" />;
                    } else {
                      <td key=p' className="graph-table__cell">
                        {cell(p, p')}
                      </td>;
                    }
                  )
                ->React.array}
             </tr>
           )
         ->React.array}
      </tbody>
    </table>;
  };
};

module GraphTable = {
  type edit =
    | SetWeight(string, string)
    | Remove
    | NotEditing;

  [@react.component]
  let make = (~vertices, ~graph, ~dispatch) => {
    let (editing, setEditing) = React.useState(() => NotEditing);
    let focused = React.useRef(Js.Nullable.null);
    React.useEffect1(
      () => {
        focusRef(focused);
        None;
      },
      [|editing|],
    );
    <div>
      <div>
        {switch (editing) {
         | Remove =>
           <button onClick={_ => setEditing(_ => NotEditing)}>
             "Done"->React.string
           </button>
         | SetWeight(_, _)
         | NotEditing =>
           <button onClick={_ => setEditing(_ => Remove)}>
             "Remove"->React.string
           </button>
         }}
      </div>
      <Table
        vertices
        cell={(i, j) =>
          switch (editing) {
          | SetWeight(i', j') when i == i' && j == j' =>
            <MatchAdder
              dispatch
              i
              j
              focused
              w={Graph.getEdge(graph, i, j)}
              onBlur={_ => setEditing(_ => NotEditing)}
            />
          | SetWeight(_, _)
          | NotEditing =>
            <button
              className="graph-table__button"
              onClick={e => {
                ReactEvent.Mouse.preventDefault(e);
                setEditing(_ => SetWeight(i, j));
              }}>
              {switch (Graph.getEdge(graph, i, j)) {
               | None => React.null
               | Some(weight) => weight->Js.String.make->React.string
               }}
            </button>
          | Remove =>
            switch (Graph.getEdge(graph, i, j)) {
            | None => React.null
            | Some(_) =>
              <button onClick={_ => dispatch(Graph.RemoveEdge(i, j))}>
                "x"->React.string
              </button>
            }
          }
        }
      />
    </div>;
  };
};

module MatchTable = {
  [@react.component]
  let make = (~graph, ~vertices) => {
    let (matches, setMatches) =
      React.useState(() => graph->Graph.toList->Blossom.Match.String.make);
    let (cardinality, setCardinality) = React.useState(() => `NotMax);
    React.useEffect2(
      () => {
        setMatches(state =>
          switch (
            graph->Graph.toList->(Blossom.Match.String.make(~cardinality))
          ) {
          | exception e =>
            Js.Console.error(e);
            graph->Graph.toList->List.toArray->Js.log;
            state;
          | matches => matches
          }
        );
        None;
      },
      (graph, cardinality),
    );
    <div>
      <div>
        <label>
          "Maximum cardinality "->React.string
          <input
            type_="checkbox"
            checked={
              switch (cardinality) {
              | `Max => true
              | `NotMax => false
              }
            }
            onChange={e =>
              if (ReactEvent.Form.currentTarget(e)##checked) {
                setCardinality(_ => `Max);
              } else {
                setCardinality(_ => `NotMax);
              }
            }
          />
        </label>
      </div>
      <Table
        vertices
        cell={(i, j) =>
          switch (Blossom.Match.get(matches, i)) {
          | Some(j') when j' == j => {j|✅|j}->Js.String.make->React.string
          | _ => React.null
          }
        }
      />
    </div>;
  };
};

module People = {
  [@react.component]
  let make = (~vertices, ~dispatch) => {
    <div>
      <ul>
        {vertices
         ->Array.map(p =>
             <li key=p>
               p->React.string
               <button onClick={_ => dispatch(Graph.RemoveVertex(p))}>
                 "x"->React.string
               </button>
             </li>
           )
         ->React.array}
      </ul>
      <PersonAdder dispatch />
    </div>;
  };
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

[@react.component]
let make = () => {
  let (graph, dispatch) = React.useReducer(Graph.reducer, sampleGraph);
  let vertices = Graph.verticesToArray(graph);
  <div style=Css.(style([display(`flex)]))>
    <div> <h2> "People"->React.string </h2> <People vertices dispatch /> </div>
    <div>
      <h2> "Potential matches"->React.string </h2>
      <GraphTable vertices graph dispatch />
    </div>
    <div>
      <h2> "Matches"->React.string </h2>
      <MatchTable graph vertices />
    </div>
  </div>;
};
