import { Canvas } from "@react-three/fiber";

function PlaceholderWorld() {
  return (
    <>
      <ambientLight intensity={1.5} />
      <directionalLight position={[4, 8, 4]} intensity={2} castShadow />
      <mesh receiveShadow rotation={[-Math.PI / 2, 0, 0]}>
        <planeGeometry args={[30, 30]} />
        <meshStandardMaterial color="#557a46" />
      </mesh>
      <mesh castShadow position={[0, 1, 0]}>
        <boxGeometry args={[1, 2, 1]} />
        <meshStandardMaterial color="#b7a58e" />
      </mesh>
    </>
  );
}

export default function App() {
  return (
    <main className="game-shell">
      <Canvas shadows camera={{ position: [5, 4, 7], fov: 60 }}>
        <PlaceholderWorld />
      </Canvas>
      <div className="prototype-label">
        <strong>The Last Side Quest</strong>
        <span>Prototype world loaded. Mildred is already judging us.</span>
      </div>
    </main>
  );
}
