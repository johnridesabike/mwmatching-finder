open Belt;

module VertexForm = [%form
  type input = {
    name: string,
    delete: bool,
    oldName: Graph.Vertex.t,
    existingNames: Graph.Vertex.Set.t,
  };
  type output = {
    name: Graph.Vertex.t,
    delete: bool,
    oldName: Graph.Vertex.t,
    existingNames: Graph.Vertex.Set.t,
  };
  let validators = {
    name: {
      strategy: OnFirstBlur,
      validate: ({name, oldName, existingNames, _}) =>
        switch (Graph.Vertex.make(name)) {
        | None => Error("Name cannot be blank.")
        | Some(name) when Graph.Vertex.eq(name, oldName) => Ok(name)
        | Some(name) when Graph.Vertex.Set.has(existingNames, name) =>
          Error(
            "Name \""
            ++ Graph.Vertex.toString(name)
            ++ "\" is already taken. Delete that vertex first.",
          )
        | Some(name) => Ok(name)
        },
    },
    delete: None,
    oldName: None,
    existingNames: None,
  }
];

module EdgeForm = [%form
  type input = {
    weight: string,
    delete: bool,
  };
  type output = {
    weight: float,
    delete: bool,
  };
  let validators = {
    weight: {
      strategy: OnFirstChange,
      validate: ({weight, _}) =>
        Ok(weight->Float.fromString->Option.getWithDefault(0.)),
    },
    delete: None,
  }
];

let useAutoFocus = () => {
  let focused = React.useRef(Js.Nullable.null);
  React.useEffect1(
    () => {
      focused
      ->React.Ref.current
      ->Js.Nullable.toOption
      ->Option.flatMap(Webapi.Dom.Element.asHtmlElement)
      ->Option.map(Webapi.Dom.HtmlElement.focus)
      ->ignore;
      None;
    },
    [|focused|],
  );
  focused;
};

module VertexEditor = {
  [@react.component]
  let make = (~dispatch, ~onSubmit, ~name, ~names) => {
    let focused = useAutoFocus();
    open VertexForm;
    let form =
      useForm(
        ~initialInput={
          name: Graph.Vertex.toString(name),
          oldName: name,
          delete: false,
          existingNames: names,
        },
        ~onSubmit=({name, delete, oldName, _}, form) => {
          if (delete) {
            dispatch(Graph.RemoveVertex(name));
          } else {
            dispatch(Graph.RenameVertex({old: oldName, new_: name}));
          };
          onSubmit();
          form.Formality.notifyOnSuccess(None);
          form.Formality.reset();
        },
      );
    <form
      onSubmit={event => {
        event->ReactEvent.Form.preventDefault;
        form.submit();
      }}>
      <label className="dialog__label">
        <p className="font-small">
          {"Rename "
           ++ Graph.Vertex.toString(form.input.oldName)
           ++ ":"
           |> React.string}
        </p>
        <input
          type_="text"
          value={form.input.name}
          disabled={form.submitting}
          onBlur={_ => form.blurName()}
          size=10
          ref={ReactDOMRe.Ref.domRef(focused)}
          onChange={event => {
            let name = event->ReactEvent.Form.target##value;
            form.updateName(input => {...input, name});
          }}
        />
      </label>
      <p>
        <label className="dialog__label">
          <span className="font-small">
            {"Delete " ++ Graph.Vertex.toString(name) ++ ": " |> React.string}
          </span>
          <input
            type_="checkbox"
            checked={form.input.delete}
            onChange={event => {
              let delete = event->ReactEvent.Form.target##checked;
              form.updateDelete(input => {...input, delete});
            }}
          />
        </label>
      </p>
      <button
        disabled={form.submitting}
        className=Cn.(
          make(["dialog__button-delete"->ifTrue(form.input.delete)])
        )>
        {switch (form) {
         | {submitting: true, _} => "Submitting..."
         | {input: {delete: true, _}, _} => "Delete"
         | _ => "Submit"
         }}
        ->React.string
      </button>
      {switch (form.nameResult) {
       | Some(Error(message)) => <p> message->React.string </p>
       | Some(Ok(_))
       | None => React.null
       }}
    </form>;
  };
};

module VertexAdder = {
  [@react.component]
  let make = (~dispatch, ~onSubmit, ~names) => {
    let focused = useAutoFocus();
    open VertexForm;
    let form =
      useForm(
        ~initialInput={
          name: "",
          oldName: Graph.Vertex.empty,
          delete: false,
          existingNames: names,
        },
        ~onSubmit=({name, _}, form) => {
          dispatch(Graph.AddVertex(name));
          onSubmit();
          form.Formality.notifyOnSuccess(None);
          form.Formality.reset();
        },
      );
    <form
      onSubmit={event => {
        event->ReactEvent.Form.preventDefault;
        form.submit();
      }}>
      <label className="dialog__label">
        <p className="font-small"> "Name vertex:"->React.string </p>
        <input
          type_="text"
          value={form.input.name}
          disabled={form.submitting}
          onBlur={_ => form.blurName()}
          size=10
          ref={ReactDOMRe.Ref.domRef(focused)}
          onChange={event => {
            let name = event->ReactEvent.Form.target##value;
            form.updateName(input => {...input, name});
          }}
        />
      </label>
      " "->React.string
      <button disabled={form.submitting}>
        (form.submitting ? "Submitting..." : "Submit")->React.string
      </button>
      {switch (form.nameResult) {
       | Some(Error(message)) => <p> message->React.string </p>
       | Some(Ok(_))
       | None => React.null
       }}
    </form>;
  };
};

module EdgeSetter = {
  [@react.component]
  let make = (~dispatch, ~onSubmit, ~i, ~j, ~w) => {
    let weight = Option.getWithDefault(w, 0.);
    let focused = useAutoFocus();
    open EdgeForm;
    let form =
      useForm(
        ~initialInput={weight: Js.String.make(weight), delete: false},
        ~onSubmit=({weight, delete}, form) => {
          if (delete) {
            dispatch(Graph.RemoveEdge(i, j));
          } else {
            dispatch(Graph.SetEdge(i, j, weight));
          };
          form.Formality.notifyOnSuccess(None);
          form.Formality.reset();
          onSubmit();
        },
      );
    <form
      onSubmit={event => {
        event->ReactEvent.Form.preventDefault;
        form.submit();
      }}>
      <label>
        <p>
          "Set the weight for the edge betweeen "->React.string
          <strong> {i->Graph.Vertex.toString->React.string} </strong>
          " and "->React.string
          <strong> {j->Graph.Vertex.toString->React.string} </strong>
          "."->React.string
        </p>
        <input
          className="match-adder__weight"
          type_="number"
          style=Css.(style([width(em(3.0))]))
          value={
            switch (form.input.weight) {
            | "" => "0"
            | x => x
            }
          }
          ref={ReactDOMRe.Ref.domRef(focused)}
          disabled={form.submitting || form.input.delete}
          onChange={event => {
            let weight = event->ReactEvent.Form.target##value;
            form.updateWeight(input => {...input, weight});
          }}
        />
      </label>
      <p className="font-small">
        <label className="dialog__label">
          "Remove "->React.string
          <input
            type_="checkbox"
            checked={form.input.delete}
            onChange={event => {
              let delete = event->ReactEvent.Form.target##checked;
              form.updateDelete(input => {...input, delete});
            }}
          />
        </label>
      </p>
      <button
        disabled={form.submitting}
        className=Cn.(
          make(["dialog__button-delete"->ifTrue(form.input.delete)])
        )>
        {switch (form) {
         | {submitting: true, _} => "Submitting..."
         | {input: {delete: true, _}, _} => "Delete"
         | _ => "Submit"
         }}
        ->React.string
      </button>
      {switch (form.weightResult) {
       | Some(Error(error)) => <p> error->React.string </p>
       | Some(Ok(_))
       | None => React.null
       }}
    </form>;
  };
};
