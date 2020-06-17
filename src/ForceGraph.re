open Belt;
open D3.Force;

let calcLinkDistance = (linkCount, ~height, ~width) => {
  let maxDimension = min(height, width);
  linkCount->( * )(5)->max(30)->min(maxDimension / 2);
};

type nodeFields = {
  id: Graph.Vertex.t,
  nodeColor: string,
  mate: option(Graph.Vertex.t),
};

type linkFields = {
  weight: float,
  linkColor: string,
  opacity: float,
};

module SvgGraph = {
  module Legend = {
    module Circle = {
      [@react.component]
      let make = (~offsetY, ~fill, ~children, ~stroke=Color.selection) =>
        <g
          className="graph-legend-circle"
          transform={j|translate(8, $offsetY)|j}>
          <circle r="8" fill strokeWidth="2" stroke />
          <text transform="translate(15, 5)" fill=Color.foreground>
            children
          </text>
        </g>;
    };

    module Line = {
      [@react.component]
      let make = (~offsetX, ~offsetY, ~stroke, ~children) =>
        <g
          className="graph-legend-line"
          transform={j|translate($offsetX, $offsetY)|j}>
          <line
            stroke
            strokeWidth="10"
            strokeLinecap="round"
            x1="0"
            y1="0"
            x2="10"
            y2="0"
          />
          <text transform="translate(20, 5)" fill=Color.foreground>
            children
          </text>
        </g>;
    };

    [@react.component]
    let make = (~offsetX, ~offsetY, ~height, ~width) =>
      <g
        className="graph-legend" transform={j|translate($offsetX, $offsetY)|j}>
        <rect
          className="graph-legend-background"
          x="0"
          y="0"
          width
          height
          rx="8"
          ry="8"
          stroke=Color.foreground
          strokeWidth="1"
          fill=Color.background
        />
        <g className="graph-legend-padding" transform="translate(7, 2)">
          <Circle offsetY=15 fill=Color.cyan>
            "Mated vertex"->React.string
          </Circle>
          <Circle offsetY=40 fill=Color.pink>
            "Single vertex"->React.string
          </Circle>
          <Line offsetX=5 offsetY=65 stroke=Color.green>
            "Matched edge"->React.string
          </Line>
          <Line offsetX=5 offsetY=90 stroke=Color.comment>
            "Unused edge"->React.string
          </Line>
        </g>
      </g>;
  };

  module InfoBox = {
    [@react.component]
    let make = (~offsetX, ~offsetY, ~children) =>
      <g
        className="graph-info-box"
        transform={j|translate($offsetX, $offsetY)|j}
        fill=Color.foreground>
        <rect
          className="graph-infoBox-background"
          x="0"
          y="0"
          width="115"
          height="75"
          rx="8"
          ry="8"
          stroke=Color.foreground
          strokeWidth="1"
          fill=Color.background
        />
        <text x="4" y="16" style=Css.(style([fontSize(px(14))]))>
          "Selection"->React.string
        </text>
        <g transform="translate(8, 36)"> children </g>
      </g>;
  };

  type selected =
    | NoneSelected
    | Single(Graph.Vertex.t)
    | Mated(Graph.Vertex.t, Graph.Vertex.t);

  type action =
    | Unselect
    | Select({
        vertex: Graph.Vertex.t,
        mate: option(Graph.Vertex.t),
      });

  let selectionReducer = (_state, action) =>
    switch (action) {
    | Unselect => NoneSelected
    | Select({vertex, mate: Some(mate)}) => Mated(vertex, mate)
    | Select({vertex, mate: None}) => Single(vertex)
    };

  [@react.component]
  let make =
      (~height, ~width as widthInt, ~links, ~nodes, ~gutterSize, ~legend) => {
    let (selected, setSelected) =
      React.Uncurried.useReducer(selectionReducer, NoneSelected);

    let totalHeight = height + gutterSize |> Js.String.make;
    let height = height->Js.String.make;
    let width = widthInt->Js.String.make;
    let circleRadius = "10";
    <svg
      xmlns="http://www.w3.org/2000/svg"
      role="img"
      alt="Network graph visualization"
      width
      height=totalHeight
      viewBox={j|0 0 $width $totalHeight|j}
      style=Css.(style([fontSize(px(12)), fontFamily(`sansSerif)]))>
      <title> "Network graph visualization"->React.string </title>
      <rect
        className="graph-background"
        width
        height
        fill=Color.background
        onTouchStart={_ => setSelected(. Unselect)}
      />
      <g className="graph-links">
        {Array.map(
           links,
           (Link.{source, target, data: {linkColor, opacity, weight}}) =>
           <line
             key={
               source.Node.data.id->Graph.Vertex.toString
               ++ "+"
               ++ target.Node.data.id->Graph.Vertex.toString
             }
             stroke=linkColor
             strokeWidth={weight->sqrt->max(1.)->min(20.)->Js.String.make}
             strokeLinecap="round"
             strokeOpacity={opacity->Js.String.make}
             x1={source.Node.x->Js.String.make}
             y1={source.Node.y->Js.String.make}
             x2={target.Node.x->Js.String.make}
             y2={target.Node.y->Js.String.make}>
             <title>
               {("Weight: " ++ weight->Js.String.make)->React.string}
             </title>
           </line>
         )
         ->React.array}
      </g>
      <g className="graph-nodes">
        {Array.map(nodes, (Node.{x, y, data: {nodeColor, id, mate}, _}) =>
           <g
             key={Graph.Vertex.toString(id)}
             onMouseOver={_ => setSelected(. Select({vertex: id, mate}))}
             onTouchStart={_ => setSelected(. Select({vertex: id, mate}))}
             onClick={_ => setSelected(. Select({vertex: id, mate}))}
             onMouseLeave={_ => setSelected(. Unselect)}>
             <circle
               transform={j|translate($x, $y)|j}
               r=circleRadius
               fill=nodeColor
               strokeWidth="2"
               stroke={
                 switch (selected) {
                 | Mated(id1, _)
                 | Single(id1) when Graph.Vertex.eq(id1, id) => Color.orange
                 | Mated(_, id2) when Graph.Vertex.eq(id2, id) => Color.purple
                 | Single(_)
                 | Mated(_, _)
                 | NoneSelected => Color.selection
                 }
               }>
               <title> {id->Graph.Vertex.toString->React.string} </title>
             </circle>
           </g>
         )
         ->React.array}
      </g>
      {legend
         ? <Legend
             offsetX={widthInt - 116}
             offsetY=height
             height="110"
             width="115"
           />
         : React.null}
      {switch (selected) {
       | Single(id) =>
         <InfoBox offsetX=128 offsetY=height>
           <Legend.Circle offsetY=0 fill=Color.pink stroke=Color.orange>
             {id->Graph.Vertex.toString->React.string}
           </Legend.Circle>
         </InfoBox>
       | Mated(id1, id2) =>
         <InfoBox offsetX=128 offsetY=height>
           <Legend.Circle offsetY=0 fill=Color.cyan stroke=Color.orange>
             {id1->Graph.Vertex.toString->React.string}
           </Legend.Circle>
           <Legend.Circle offsetY=24 fill=Color.cyan stroke=Color.purple>
             {id2->Graph.Vertex.toString->React.string}
           </Legend.Circle>
         </InfoBox>
       | NoneSelected => React.null
       }}
    </svg>;
  };
};

[@react.component]
let make =
    (
      ~graph,
      ~mates,
      ~width,
      ~height,
      ~children as caption=React.null,
      ~style=ReactDOMRe.Style.make(),
      ~legend=true,
    ) => {
  let nodes =
    graph
    ->Graph.verticesToArray
    ->Array.map(id => {
        let mate = Blossom.Match.get(mates, id);
        let nodeColor = Option.isSome(mate) ? Color.cyan : Color.pink;
        MakeNode.make(~data={id, nodeColor, mate});
      });
  let links =
    graph
    ->Graph.toList
    ->List.map(((i, j, weight)) => {
        let (linkColor, opacity) =
          switch (Blossom.Match.get(mates, i)) {
          | Some(mate) when Graph.Vertex.eq(mate, j) => (Color.green, 1.0)
          | _ => (Color.comment, 0.5)
          };
        MakeLink.make(
          ~source=Graph.Vertex.toString(i),
          ~target=Graph.Vertex.toString(j),
          ~data={weight, linkColor, opacity},
        );
      })
    ->List.toArray;

  let charge =
    ()
    ->ManyBody.make
    ->ManyBody.strength(() => (-180.))
    ->ManyBody.distanceMin(10.)
    ->ManyBody.distanceMax(min(height, width) / 2 |> Float.fromInt);
  //let charge = Radial.make(100.);
  let forceLink =
    links
    ->Links.make
    ->Links.id(d => Graph.Vertex.toString(d.Node.data.id))
    ->Links.distance(_ =>
        links->Array.size->calcLinkDistance(~height, ~width)
      );
  let simulation =
    nodes
    ->Simulation.make
    ->Simulation.setForceLink(forceLink)
    ->Simulation.setForceChargeManyBody(charge)
    //->Simulation.setForceChargeRadial(charge)
    ->Simulation.setForceCenter(
        Center.make(~width=width / 2, ~height=height / 2),
      )
    ->Simulation.tick(180);

  let nodes = simulation->Simulation.getNodes;
  let links = forceLink->Links.unsafeToArray;

  <figure className="force-graph" style>
    <SvgGraph width height gutterSize=126 links nodes legend />
    <figcaption className="force-graph__caption"> caption </figcaption>
  </figure>;
};
