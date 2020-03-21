open Belt;

type dialog =
  | Closed
  | EditingVertex(string)
  | NewVertex
  | EditingEdge(string, string, option(float))
  | Reset;

let setClosed = _ => Closed;

module Dialog = {
  [@react.component]
  let make = (~children, ~ariaLabel, ~onDismiss) =>
    <Dialog className="dialog__edit" onDismiss ariaLabel>
      <button onClick={_ => onDismiss()} className="dialog__close-button">
        "Close"->React.string
      </button>
      children
    </Dialog>;
};

module Table = {
  [@react.component]
  let make =
      (
        ~vertices,
        ~editVertex,
        ~editEdge,
        ~mates,
        ~graph,
        ~disabled=false,
        ~style=ReactDOMRe.Style.make(),
      ) => {
    let verticesIndices = Array.size(vertices) - 1;
    /*         👆 Now there's a fun name. */
    <table className="graph-table" style>
      <tbody>
        <tr>
          <th className="graph-table__corner font-small">
            React.null
          </th>
          {vertices
           ->Array.reverse
           ->Array.slice(~len=verticesIndices, ~offset=0)
           ->Array.map(p =>
               <th key=p className="graph-table__top-name" scope="col">
                 <button
                   className="graph-table__name-button"
                   onClick={_ => editVertex(p)}
                   disabled>
                   <div className="vertical-lr"> p->React.string </div>
                 </button>
               </th>
             )
           ->React.array}
        </tr>
        {vertices
         ->Array.slice(~len=verticesIndices, ~offset=0)
         ->Array.mapWithIndex((index, i) =>
             <tr key=i>
               <th className="graph-table__side-name" scope="row">
                 <button
                   className="graph-table__name-button"
                   onClick={_ => editVertex(i)}
                   disabled>
                   i->React.string
                 </button>
               </th>
               {vertices
                ->Array.reverse
                ->Array.slice(~len=verticesIndices, ~offset=0)
                ->Array.mapWithIndex((index', j) =>
                    if (verticesIndices - index' <= index) {
                      <td key=j className="graph-table__null-cell" />;
                    } else {
                      let mated =
                        switch (Blossom.Match.get(mates, i)) {
                        | Some(j') when j == j' => true
                        | _ => false
                        };
                      <td
                        key=j
                        className=Cn.(
                          make([
                            "graph-table__cell",
                            "graph-table__cell-mated"->ifTrue(mated),
                          ])
                        )>
                        {switch (Graph.getEdge(graph, i, j)) {
                         | None =>
                           <button
                             disabled
                             title="Edge not set."
                             className="graph-table__button"
                             onClick={_ => editEdge(i, j, None)}>
                             <VisuallyHidden>
                               (mated ? "Mated. " : "")->React.string
                               {j|Set weight for $i and $j.|j}->React.string
                             </VisuallyHidden>
                           </button>
                         | Some(weight) =>
                           <button
                             disabled
                             title={mated ? "Mated. " : "Not mated."}
                             className="graph-table__button mated"
                             onClick={_ => editEdge(i, j, Some(weight))}>
                             <VisuallyHidden>
                               (mated ? "Mated. " : "")->React.string
                               {j|Set weight for $i and $j.|j}->React.string
                             </VisuallyHidden>
                             {weight->Js.String.make->React.string}
                           </button>
                         }}
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

/*
 module RandomButton = {
   [@react.component]
   let make = (~vertices, ~dispatch) =>
     <button
       className="toolbar__item"
       onClick={_ => {
         let randomVertices = Array.shuffle(vertices);
         let (i, j) = (randomVertices[0], randomVertices[1]);
         switch (i, j) {
         | (Some(i), Some(j)) =>
           dispatch(
             Graph.SetEdge(i, j, Js.Math.random_int(1, 100)->Int.toFloat),
           )
         | _ => ()
         };
       }}>
       "Random"->React.string
     </button>;
 };
 */

[@react.component]
let make =
    (
      ~vertices,
      ~graph,
      ~dispatch,
      ~cardinality,
      ~setCardinality,
      ~mates,
      ~style=ReactDOMRe.Style.make(),
    ) => {
  let (dialog, setDialog) = React.useState(setClosed);
  let closeDialog = _ => setDialog(setClosed);
  <div style className="graph-table-wrapper">
    <div className="toolbar">
      <button className="toolbar__item" onClick={_ => setDialog(_ => Reset)}>
        "New graph"->React.string
      </button>
      <button
        className="toolbar__item" onClick={_ => setDialog(_ => NewVertex)}>
        "Add vertex"->React.string
      </button>
      <label>
        "Maximum cardinality "->React.string
        <input
          className="toolbar__item"
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
    {switch (vertices) {
     | [||] =>
       <React.Fragment>
         <p> "No one to match. Add someone to begin."->React.string </p>
         <p>
           <button onClick={_ => dispatch(Graph.Set(SampleData.a))}>
             "Load a sample graph"->React.string
           </button>
         </p>
       </React.Fragment>
     | [|id|] =>
       <p>
         <strong> id->React.string </strong>
         " is alone. Add more people to start matching."->React.string
       </p>
     | vertices =>
       <React.Fragment>
         <Table
           vertices
           mates
           graph
           editVertex={p => setDialog(_ => EditingVertex(p))}
           editEdge={(i, j, w) => setDialog(_ => EditingEdge(i, j, w))}
         />
         {switch (Map.size(graph.Graph.edges)) {
          | 0 =>
            <p>
              "Click a table cell to add an edge to the graph."->React.string
            </p>
          | _ => React.null
          }}
       </React.Fragment>
     }}
    {switch (dialog) {
     | Closed => React.null
     | EditingVertex(person) =>
       <Dialog onDismiss=closeDialog ariaLabel={"Editing person: " ++ person}>
         <Forms.VertexEditor
           dispatch
           name=person
           names={graph.Graph.vertices}
           onSubmit=closeDialog
         />
       </Dialog>
     | NewVertex =>
       <Dialog onDismiss=closeDialog ariaLabel="Adding a new person">
         <Forms.VertexAdder
           dispatch
           names={graph.Graph.vertices}
           onSubmit=closeDialog
         />
       </Dialog>
     | EditingEdge(i, j, w) =>
       <Dialog
         onDismiss=closeDialog ariaLabel={j|Editing edge between $i and $j|j}>
         <Forms.EdgeSetter i j w dispatch onSubmit=closeDialog />
       </Dialog>
     | Reset =>
       <Dialog onDismiss=closeDialog ariaLabel="Reset graph options">
         <p style=Css.(Css.style([textAlign(`center)]))>
           <button
             className="dialog__button-delete"
             onClick={_ => {
               dispatch(Graph.Set(SampleData.a));
               closeDialog();
             }}>
             "Reset to sample data"->React.string
           </button>
         </p>
         <p style=Css.(Css.style([textAlign(`center)]))>
           <button
             className="dialog__button-delete"
             onClick={_ => {
               dispatch(Graph.Set(Graph.empty));
               closeDialog();
             }}>
             "Reset to blank data"->React.string
           </button>
         </p>
       </Dialog>
     }}
  </div>;
};
